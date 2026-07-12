/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:08 by fbendnan          #+#    #+#             */
/*   Updated: 2026/06/17 12:07:13 by fbendnan         ###   ########.fr       */
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

static int	check_burnout(t_coder *cdr, int n, long burnout_time, long start, t_simulation	*sim)
{
	int		i;
	long	now;

	now = get_time_in_ms();
	i = 0;
	while (i < n)
	{
		if (cdr[i].last_time_compilation != 0
			&& now - cdr[i].last_time_compilation >= burnout_time
			&& cdr[i].compiles_done < cdr[i].infos->number_of_compiles_required)
		{
			pthread_mutex_lock(&cdr[i].infos->print_mutex);
			printf("%ld %d burned out\n", get_time_in_ms() - start, cdr[i].id);
			sim->running = 0;
			pthread_mutex_unlock(&cdr[i].infos->print_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	should_stop(t_monitor *monitor, t_simulation	*sim)
{
	t_coder			*coders;
	t_shared_info	*info;
	long			start_time;

	coders = monitor->coders;
	info = monitor->info;
	start_time = info->start_time;
	if (check_burnout(coders, info->number_of_coders, info->time_to_burnout,
			start_time, sim) || all_coders_done(coders, info->number_of_coders,
			info->number_of_compiles_required))
		return (1);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_monitor		*monitor;
	t_simulation	*sim;

	monitor = (t_monitor *)arg;
	sim = monitor->sim;
	while (1)
	{
		pthread_mutex_lock(&sim->mutex);
		if (should_stop(monitor, sim))
		{
			sim->running = 0;
			pthread_mutex_unlock(&sim->mutex);
			exit(0);
		}
		pthread_mutex_unlock(&sim->mutex);
		usleep(500);
	}
	return (NULL);
}
