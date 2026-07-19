/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:15:24 by fbendnan          #+#    #+#             */
/*   Updated: 2026/07/19 23:53:25 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_sim_running(t_coder *coder)
{
	int	r;

	pthread_mutex_lock(&coder->sim->mutex);
	r = coder->sim->running;
	pthread_mutex_unlock(&coder->sim->mutex);
	return (r);
}

static int	waiting_room(t_dongle *d, t_coder *coder)
{
	t_request_node	*req;

	req = malloc(sizeof(t_request_node));
	if (!req)
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	req->coder_id = coder->id;
	req->priority = get_request_priority(coder);
	req->personal_cond = &coder->personal_cond;
	req->personal_mutex = &coder->personal_mutex;
	req->next = NULL;
	queue_insert(d, req, coder->infos->scheduler);
	pthread_mutex_lock(&coder->personal_mutex);
	pthread_mutex_unlock(&d->mutex);
	pthread_cond_wait(&coder->personal_cond, &coder->personal_mutex);
	pthread_mutex_unlock(&coder->personal_mutex);
	return (1);
}

static int	take_dongle_and_log(t_dongle *d, t_coder *coder)
{
	d->in_use = 1;
	pthread_mutex_unlock(&d->mutex);

	if (!is_sim_running(coder))
	{
		pthread_mutex_lock(&d->mutex);
		d->in_use = 0;
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}

	pthread_mutex_lock(&coder->infos->print_mutex);
	printf("%ld %d has taken a dongle\n",
		get_time_in_ms() - coder->infos->start_time, coder->id);
	pthread_mutex_unlock(&coder->infos->print_mutex);
	return (1);
}

int	dongle_take(t_dongle *d, t_coder *coder)
{
	long	now;

	if (!is_sim_running(coder))
		return (0);

	pthread_mutex_lock(&d->mutex);
	now = get_time_in_ms();

	if (!d->in_use && now >= d->cooldown_until && !d->wait_queue)
		return (take_dongle_and_log(d, coder));

	if (!d->in_use && now < d->cooldown_until && !d->wait_queue)
	{
		while (now < d->cooldown_until)
		{
			pthread_mutex_unlock(&d->mutex);
			usleep(100);
			pthread_mutex_lock(&d->mutex);
			now = get_time_in_ms();
		}
		return (take_dongle_and_log(d, coder));
	}

	if (!waiting_room(d, coder))
		return (0);

	pthread_mutex_lock(&d->mutex);
	return (take_dongle_and_log(d, coder));
}

void	dongle_release(t_dongle *d, t_shared_info *info)
{
	t_request_node	*next;

	pthread_mutex_lock(&d->mutex);
	d->cooldown_until = get_time_in_ms() + info->time_to_cooldown;
	next = queue_pop(d);
	if (next)
		d->in_use = 1;
	else
		d->in_use = 0;
	pthread_mutex_unlock(&d->mutex);

	if (next)
	{
		pthread_mutex_lock(next->personal_mutex);
		pthread_cond_signal(next->personal_cond);
		pthread_mutex_unlock(next->personal_mutex);
		free(next);
	}
}