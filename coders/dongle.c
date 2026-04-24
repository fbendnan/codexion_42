/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:29:33 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 16:41:06 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int dongle_take(t_dongle *d, t_coder *coder)
{
    t_request_node *req_node;
    long now;
    pthread_mutex_lock(&d->mutex);

    now = get_time_in_ms();
    if (!d->in_use && now >= d->cooldown_until)
    {
        d->in_use = 1;
        pthread_mutex_unlock(&d->mutex);
        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%ld %d has taken a dongle\n",
               now - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);
        return (1);
    }
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
    pthread_mutex_lock(&d->mutex);
    while ((now = get_time_in_ms()) < d->cooldown_until)
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

void dongle_release(t_dongle *d, t_shared_info *info)
{
    t_request_node *next_node;
    
    pthread_mutex_lock(&d->mutex);
    d->in_use = 0;
    d->cooldown_until = get_time_in_ms() + info->dongle_cooldown;
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
