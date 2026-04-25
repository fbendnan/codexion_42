/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:29:33 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/25 12:11:22 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
//to check

int	waiting_room(t_dongle *d, t_coder *coder)
{
	t_request_node	*req_node;

	req_node = malloc(sizeof(t_request_node));
	if (!req_node)
		return (pthread_mutex_unlock(&d->mutex), 0);
	req_node->coder_id = coder->id;
	req_node->priority = get_request_priority(coder);
	req_node->personal_cond = &coder->personal_cond;
	req_node->personal_mutex = &coder->personal_mutex;
	req_node->next = NULL;
	queue_insert(d, req_node, coder->infos->scheduler);
	pthread_mutex_lock(&coder->personal_mutex);
	pthread_mutex_unlock(&d->mutex);
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
	if(!waiting_room(d, coder))
		return (0);
	pthread_mutex_lock(&d->mutex);
	while (get_time_in_ms() < d->cooldown_until || d->in_use)
	{
		pthread_mutex_unlock(&d->mutex);
		usleep(100);
		pthread_mutex_lock(&d->mutex);
	}
	d->in_use = 1;
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
	d->in_use = 0;
	d->cooldown_until = get_time_in_ms() + info->time_to_cooldown;
	next_node = queue_pop(d);
	pthread_mutex_unlock(&d->mutex);
	if (next_node)
	{
		pthread_mutex_lock(next_node->personal_mutex);
		pthread_cond_signal(next_node->personal_cond);
		pthread_mutex_unlock(next_node->personal_mutex);
		free(next_node);
	}
}
