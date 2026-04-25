#include "codexion.h"

int	waiting_room(t_dongle *d, t_coder *coder)
{
	t_request_node	*req_node;

	req_node = malloc(sizeof(t_request_node));
	if (!req_node)
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	req_node->coder_id = coder->id;
	req_node->priority = get_request_priority(coder);
	req_node->personal_cond = &coder->personal_cond;
	req_node->personal_mutex = &coder->personal_mutex;
	req_node->next = NULL;

	queue_insert(d, req_node, coder->infos->scheduler);

	/* Lock personal mutex *before* releasing dongle to avoid lost signal */
	pthread_mutex_lock(&coder->personal_mutex);
	pthread_mutex_unlock(&d->mutex);

	/* Sleep – the release function will signal us */
	pthread_cond_wait(&coder->personal_cond, &coder->personal_mutex);
	pthread_mutex_unlock(&coder->personal_mutex);

	return (1);
}

int	dongle_take(t_dongle *d, t_coder *coder)
{
	long	now;

	pthread_mutex_lock(&d->mutex);
	now = get_time_in_ms();

	if (!d->in_use && now >= d->cooldown_until && !d->wait_queue)
	{
		d->in_use = 1;
		pthread_mutex_unlock(&d->mutex);
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%ld %d has taken a dongle\n",
			now - coder->infos->start_time, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);
		return (1);
	}
	if (!d->in_use && now < d->cooldown_until && !d->wait_queue)
	{
		while (now < d->cooldown_until)
		{
			pthread_mutex_unlock(&d->mutex);
			usleep(100);
			pthread_mutex_lock(&d->mutex);
			now = get_time_in_ms();
		}
		d->in_use = 1;
		pthread_mutex_unlock(&d->mutex);
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%ld %d has taken a dongle\n",
			now - coder->infos->start_time, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);
		return (1);
	}
	if (!waiting_room(d, coder))
		return (0);
	pthread_mutex_lock(&d->mutex);
	now = get_time_in_ms();
	while (now < d->cooldown_until)
	{
		pthread_mutex_unlock(&d->mutex);
		usleep(100);
		pthread_mutex_lock(&d->mutex);
		now = get_time_in_ms();
	}
	pthread_mutex_unlock(&d->mutex);
	pthread_mutex_lock(&coder->infos->print_mutex);
	printf("%ld %d has taken a dongle\n",
		get_time_in_ms() - coder->infos->start_time, coder->id);
	pthread_mutex_unlock(&coder->infos->print_mutex);
	return (1);
}

void	dongle_release(t_dongle *d, t_shared_info *info)
{
	t_request_node	*next_node;

	pthread_mutex_lock(&d->mutex);
	d->cooldown_until = get_time_in_ms() + info->time_to_cooldown;
	next_node = queue_pop(d);
	if (next_node)
		d->in_use = 1;
	else
		d->in_use = 0;
	pthread_mutex_unlock(&d->mutex);
	if (next_node)
	{
		pthread_mutex_lock(next_node->personal_mutex);
		pthread_cond_signal(next_node->personal_cond);
		pthread_mutex_unlock(next_node->personal_mutex);
		free(next_node);
	}
}
