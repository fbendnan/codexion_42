/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:56 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 16:42:11 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//init.c

#include "codexion.h"

void initialize_dongles(int number_of_dongles, t_dongle *dongles)
{
    int i;

    i = 0;
    while (i < number_of_dongles)
    {
        dongles[i].id = i;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        dongles[i].in_use = 0;
        dongles[i].cooldown_until = 0;
        dongles[i].wait_queue = NULL;
        i++;
    }
}

void initialize_coders(t_shared_info *infos, t_dongle *dongles, t_coder *coders, t_simulation *sim)
{
    int i;

    i = 0;
    while (i < infos->number_of_coders)
    {
        coders[i].id = i + 1;
        coders[i].compiles_done = 0;
        coders[i].state = 0;
        coders[i].last_time_compilation = infos->start_time;
        coders[i].right_dongle = &dongles[(i + 1) % infos->number_of_coders];
        coders[i].left_dongle = &dongles[i];
        coders[i].infos = infos;
        coders[i].sim_running = &sim->running;
        coders[i].sim_mutex = &sim->mutex;
        pthread_cond_init(&coders[i].personal_cond, NULL);
        pthread_mutex_init(&coders[i].personal_mutex, NULL);
        i++;
    }
}

void init_simulation(t_simulation *sim)
{
    sim->running = 1;
    pthread_mutex_init(&sim->mutex, NULL);
}
