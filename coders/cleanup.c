/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:42 by fbendnan          #+#    #+#             */
/*   Updated: 2026/06/17 12:24:36 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./codexion 2 2000 100 100 100 2 50 fifo

#include "codexion.h"

void	cleanup(t_dongle *dongles, t_coder *coders,
	t_shared_info *info, t_simulation *sim)
{
	int				i;
	t_request_node	*tmp;
	t_request_node	*next;

	i = 0;
	while (i < info->number_of_coders)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		pthread_cond_destroy(&coders[i].personal_cond);
		pthread_mutex_destroy(&coders[i].personal_mutex);
		tmp = dongles[i].wait_queue;
		while (tmp)
		{
			next = tmp->next;
			free(tmp);
			tmp = next;
		}
		i++;
	}
	pthread_mutex_destroy(&sim->mutex);
	pthread_mutex_destroy(&sim->print_mutex);
	free(info->scheduler);
	free(dongles);
	free(coders);
}
