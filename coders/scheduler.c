/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:29:42 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 22:58:25 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_request_priority(t_coder *coder)
{
    if (strcmp(coder->infos->scheduler, "FIFO") == 0)
        return (get_time_in_ms());
    else
        return (coder->last_time_compilation + coder->infos->time_to_burnout);
}

t_request_node *queue_pop(t_dongle *d)
{
    t_request_node *first;

    first = d->wait_queue;
    if (first)
        d->wait_queue = first->next;
    return (first);
}

void queue_insert(t_dongle *d, t_request_node *new_node, char *scheduler)
{
    t_request_node *curr;
    t_request_node *prev;
    new_node->next = NULL;
    if (!d->wait_queue)
    {
        d->wait_queue = new_node;
        return;
    }
    if (strcmp(scheduler, "FIFO") == 0)
    {
        curr = d->wait_queue;
        while (curr->next)
            curr = curr->next;
        curr->next = new_node;
    }
    else
    {
        prev = NULL;
        curr = d->wait_queue;
        while (curr && curr->priority <= new_node->priority)
        {
            prev = curr;
            curr = curr->next;
        }
        if (!prev)
        {
            new_node->next = d->wait_queue;
            d->wait_queue = new_node;
        }
        else
        {
            prev->next = new_node;
            new_node->next = curr;
        }
    }
}
