/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 12:27:22 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 13:07:21 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main_process(t_shared_info *info)
{
	t_dongle		*dongles;
	t_coder			*coders;
	t_simulation	sim;
	t_monitor		monitor;
	pthread_t		monitor_thread;

	dongles = malloc(sizeof(t_dongle) * info->number_of_coders);
	if (!dongles)
		return (display_error("Error: malloc failed\n"), 0);
	initialize_dongles(info->number_of_coders, dongles);
	init_simulation(&sim);
	coders = malloc(sizeof(t_coder) * info->number_of_coders);
	if (!coders)
		return(display_error("Error: malloc failed\n"), free(dongles), 0);
	initialize_coders(info, dongles, coders, &sim);
	create_coders(coders);
	monitor.coders = coders;
	monitor.info = info;
	monitor.sim = &sim;
	pthread_create(&monitor_thread, NULL, monitor_routine, &monitor);
	wait_coders_creation(coders);
	pthread_join(monitor_thread, NULL);
	cleanup(dongles, coders, info, &sim);
	return (1);
}

int	main(int argc, char **argv)
{
	t_shared_info	info;
		
	if (!parsing_codexion(argc, argv, &info))
		return (1);
	info.start_time = get_time_in_ms();
	pthread_mutex_init(&info.print_mutex, NULL);
	if (!main_process(&info))
		return (1);
	return (0);
}