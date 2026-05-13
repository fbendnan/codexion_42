/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:15:39 by fbendnan          #+#    #+#             */
/*   Updated: 2026/05/13 07:21:56 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
