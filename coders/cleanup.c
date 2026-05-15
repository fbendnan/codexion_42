/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:42 by fbendnan          #+#    #+#             */
/*   Updated: 2026/05/15 08:14:39 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc -Wall -Wextra -Werror -pthread *.c -o codexion
// ./codexion 2 2000 100 100 100 2 50 fifo

#include "codexion.h"

void cleanup(t_dongle *dongles, t_coder *coders, t_shared_info *info, t_simulation *sim)
{
    int             i;
    t_request_node  *tmp;
    t_request_node  *next;

    i = 0;
    while (i < info->number_of_coders)
    {
        pthread_mutex_destroy(&dongles[i].mutex);
        pthread_cond_destroy(&dongles[i].cond);
        pthread_cond_destroy(&coders[i].personal_cond);
        pthread_mutex_destroy(&coders[i].personal_mutex);

        // Free any remaining request nodes in the dongle's wait queue
        tmp = dongles[i].wait_queue;
        while (tmp)
        {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }
        i++;
    }

    // Destroy print mutex
    pthread_mutex_destroy(&info->print_mutex);

    // Destroy simulation mutex
    pthread_mutex_destroy(&sim->mutex);

    // Free the scheduler string
    free(info->scheduler);

    // Free the arrays
    free(dongles);
    free(coders);
}
