/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:29:52 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 13:29:53 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *start_simulation(void *argv)
{
    t_coder  *coder;
    t_dongle *first;
    t_dongle *second;
    int      keep_running;

    coder = (t_coder *)argv;
    // Decide lock order to prevent deadlock
    if (coder->id % 2 == 0)
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    else
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }

    while (1)
    {
        //Check global stop flag (set by monitor thread)
        pthread_mutex_lock(coder->sim_mutex);
        keep_running = *(coder->sim_running);
        pthread_mutex_unlock(coder->sim_mutex);
        if (!keep_running)
            break;

        // Try to acquire both dongles (with proper cooldown)
        if (!dongle_take(first, coder))
            break; // Should never fail, but safety
        if (!dongle_take(second, coder))
        {
            dongle_release(first, coder->infos);
            break;
        }

        coder->last_time_compilation = get_time_in_ms();

        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%li %i is compiling\n",
               get_time_in_ms() - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);

        usleep(coder->infos->time_to_compile * 1000);
        coder->compiles_done++;

        dongle_release(first, coder->infos);
        dongle_release(second, coder->infos);

        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%li %i is debugging\n",
               get_time_in_ms() - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);

        usleep(coder->infos->time_to_debug * 1000);

        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%li %i is refactoring\n",
               get_time_in_ms() - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);
        usleep(coder->infos->time_to_refactor * 1000);
    }
    return (NULL);
}

void create_coders(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders->infos->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, start_simulation, &coders[i]);
		i++;
	}
}

void wait_coders_creation(t_coder *coders)
{
	int	i;

    i = 0;
	while (i < coders->infos->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
}
