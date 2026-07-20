/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:08 by fbendnan          #+#    #+#             */
/*   Updated: 2026/07/19 22:48:38 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_coders_done(t_monitor *monitor, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < monitor->info->number_of_coders)
	{
		pthread_mutex_lock(&sim->mutex);
		if (monitor->coders[i].compiles_done
			< monitor->info->number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->mutex);
			return (0);
		}
		pthread_mutex_unlock(&sim->mutex);
		i++;
	}
	return (1);
}

static int	check_burnout(t_monitor *monitor, t_simulation *sim)
{
	int				i;
	long			now;
	t_coder			*coder;
	t_shared_info	*info;

	coder = monitor->coders;
	info = monitor->info;
	now = get_time_in_ms();
	i = 0;
	while (i < info->number_of_coders)
	{
		pthread_mutex_lock(&sim->mutex);
		if (coder[i].compiles_done < info->number_of_compiles_required
			&& now - coder[i].last_time_compilation >= info->time_to_burnout)
		{
			pthread_mutex_unlock(&sim->mutex);
			print_info(coder, "burned out");
			// printf("comp = %li, right = %i, left = %i\n",coder[i].compiles_done, coder[i].right_dongle->id, coder[i].left_dongle->id);
			return (1);
		}
		pthread_mutex_unlock(&sim->mutex);
		i++;
	}
	return (0);
}

static void	wake_all_coders(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->info->number_of_coders)
	{
		pthread_mutex_lock(&monitor->coders[i].personal_mutex);
		pthread_cond_signal(&monitor->coders[i].personal_cond);
		pthread_mutex_unlock(&monitor->coders[i].personal_mutex);
		i++;
	}
}

int	should_stop(t_monitor *monitor, t_simulation	*sim)
{
	if (check_burnout(monitor, sim) || all_coders_done(monitor, sim))
	{
		pthread_mutex_lock(&sim->mutex);
		sim->running = 0;
		pthread_mutex_unlock(&sim->mutex);
		wake_all_coders(monitor);
		return (1);
	}
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
		if (should_stop(monitor, sim))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
