/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnane <fbendnane@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:29:52 by fbendnan          #+#    #+#             */
/*   Updated: 2026/07/19 23:50:29 by fbendnane        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	decide_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->id % 2 == 0)
	{
		*(first) = coder->right_dongle;
		*(second) = coder->left_dongle;
	}
	else
	{
		*(first) = coder->left_dongle;
		*(second) = coder->right_dongle;
	}
}

void one_coder(t_coder *coder)
{
	print_info(coder, "has taken a dongle");
	precise_usleep(coder->infos->time_to_burnout, coder->sim);
}

void	compile_and_relase_dongles(
	t_coder *coder, t_dongle *first, t_dongle *second)
{

	print_info(coder, "is compiling");
	precise_usleep(coder->infos->time_to_compile, coder->sim);
	pthread_mutex_lock(&coder->sim->mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->sim->mutex);
	dongle_release(first, coder->infos);
	dongle_release(second, coder->infos);
}

void	debug_and_refactor(t_coder *coder)
{
	print_info(coder, "is debugging");
	precise_usleep(coder->infos->time_to_debug, coder->sim);
	print_info(coder, "is refactoring");
	precise_usleep(coder->infos->time_to_refactor, coder->sim);
}

void	*start_simulation(void *argv)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)argv;
	if (coder->infos->number_of_coders == 1)
	{
		one_coder(coder);
		return(NULL);
	}
	decide_order(coder, &first, &second);
	while (1)
	{
		pthread_mutex_lock(&coder->sim->mutex);
		if (!coder->sim->running || coder->compiles_done
			>= coder->infos->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->sim->mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->sim->mutex);
		dongle_take(first, coder);
		
		dongle_take(second, coder);
		pthread_mutex_lock(&coder->sim->mutex);
		coder->last_time_compilation = get_time_in_ms();
		pthread_mutex_unlock(&coder->sim->mutex);
		compile_and_relase_dongles(coder, first, second);
		debug_and_refactor(coder);
	}
	return (NULL);
}
