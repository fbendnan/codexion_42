/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:15:24 by fbendnan          #+#    #+#             */
/*   Updated: 2026/05/13 11:56:44 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	pthread_mutex_lock(&coder->personal_mutex);
	pthread_mutex_unlock(&d->mutex);
	pthread_cond_wait(&coder->personal_cond, &coder->personal_mutex);
	pthread_mutex_unlock(&coder->personal_mutex);
	return (1);
}

void waiting_cooldown(t_dongle *d, t_coder *coder)
{
    struct timespec abstime;
    long now;
    long remaining_ms;

    now = get_time_in_ms();
    remaining_ms = d->cooldown_until - now;

    if (remaining_ms > 0)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        
        // Convert remaining ms to microseconds for calculation
        long total_usec = tv.tv_usec + (remaining_ms * 1000);
        abstime.tv_sec = tv.tv_sec + (total_usec / 1000000);
        abstime.tv_nsec = (total_usec % 1000000) * 1000;

        // Use the ALREADY LOCKED mutex d->mutex
        while (get_time_in_ms() < d->cooldown_until)
        {
            // pthread_cond_timedwait will unlock d->mutex while waiting
            // and re-lock it automatically upon wake up.
            pthread_cond_timedwait(&d->cond, &d->mutex, &abstime);
        }
    }

    // Now cooldown is finished – we already hold the mutex, claim the dongle
    d->in_use = 1;
    pthread_mutex_unlock(&d->mutex);

    // Print outside the dongle mutex to minimize contention
	if (!coder->sim->running)
		return ;
	pthread_mutex_lock(&coder->infos->print_mutex);
	printf("%ld %d has taken a dongle\n",
		get_time_in_ms() - coder->infos->start_time, coder->id);
	pthread_mutex_unlock(&coder->infos->print_mutex);
}

int	dongle_take(t_dongle *d, t_coder *coder)
{
	long	now;

	pthread_mutex_lock(&d->mutex);
	now = get_time_in_ms();
    
    // Check if dongle is free and cooldown has passed
	if (!d->in_use && now >= d->cooldown_until && !d->wait_queue)
	{
		if (!coder->sim->running)
			return (0);
		d->in_use = 1;
		pthread_mutex_unlock(&d->mutex);
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%ld %d has taken a dongle\n",
			get_time_in_ms() - coder->infos->start_time, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);
		return (1);
	}
    
    // If it's free but still in cooldown, wait for it
	if (!d->in_use && now < d->cooldown_until && !d->wait_queue)
	{
		waiting_cooldown(d, coder); // Note: waiting_cooldown unlocks d->mutex
		return (1);
	}
    
    // Otherwise, enter the wait queue
	if (!waiting_room(d, coder))
		return (0);
        
    // When waking up from waiting_room, the dongle was assigned (in_use = 1)
    // but the cooldown might still apply to the next user.
    // However, in your logic, the 'in_use' is set by the releaser if there's a queue.
    // You must ensure the cooldown logic is consistent here.
	pthread_mutex_lock(&d->mutex);
	waiting_cooldown(d, coder);
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
