/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:08 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 16:45:09 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int	all_coders_done(t_coder *coders, int n, int required)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (coders[i].compiles_done < required)
			return (0);
		i++;
	}
	return (1);
}


static int check_burnout(t_coder *coders, int n, long burnout_time, long start)
{
	int		i;
	long	now;

	now = get_time_in_ms();
	i = 0;
	while (i < n)
	{
		if (coders[i].last_time_compilation != 0
			&& now - coders[i].last_time_compilation > burnout_time)
		{
			pthread_mutex_lock(&coders[i].infos->print_mutex);
            printf("%ld %d burned out\n", now - start, coders[i].id);
            pthread_mutex_unlock(&coders[i].infos->print_mutex);
            return (1);
		}
		i++;
	}
	return (0);
}


void *monitor_routine(void *arg)
{
	t_monitor		*monitor;
	t_simulation	*sim;
	t_coder			*coders;
	t_shared_info	*info;
	long			start_time;
	int				stop;

	monitor = (t_monitor *)arg;
	sim = monitor->sim;
	coders = monitor->coders;
	info = monitor->info;
	start_time = info->start_time;
	while (1)
	{
		usleep(1000);
		pthread_mutex_lock(&sim->mutex);
		stop = 0;
		if (check_burnout(coders, info->number_of_coders, info->time_to_burnout, start_time))
			stop = 1;
		else if (all_coders_done(coders, info->number_of_coders, info->number_of_compiles_required))
			stop = 1;
		if (stop)
		{
			sim->running = 0;
			pthread_mutex_unlock(&sim->mutex);
			exit(0);
		}
		pthread_mutex_unlock(&sim->mutex);
	}
	return (NULL);
}