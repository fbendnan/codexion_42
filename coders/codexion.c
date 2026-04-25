/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:29:52 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/25 12:22:45 by fbendnan         ###   ########.fr       */
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

void	compile_and_relase_dongles(
	t_coder *coder, t_dongle *first, t_dongle *second)
{
	if (!(*(coder->sim_running)))
		return ;
	pthread_mutex_lock(&coder->infos->print_mutex);
	printf("%li %i is compiling\n",
		get_time_in_ms() - coder->infos->start_time, coder->id);
	pthread_mutex_unlock(&coder->infos->print_mutex);
	usleep(coder->infos->time_to_compile * 1000);
	coder->compiles_done++;
	dongle_release(first, coder->infos);
	dongle_release(second, coder->infos);
}

void	debug_and_refactor(t_coder *coder)
{
	if (!(*(coder->sim_running)))
		return ;
	pthread_mutex_lock(&coder->infos->print_mutex);
	printf("%li %i is debugging\n",
		get_time_in_ms() - coder->infos->start_time, coder->id);
	pthread_mutex_unlock(&coder->infos->print_mutex);
	usleep(coder->infos->time_to_debug * 1000);
	if (!(*(coder->sim_running)))
		return ;
	pthread_mutex_lock(&coder->infos->print_mutex);
	printf("%li %i is refactoring\n",
		get_time_in_ms() - coder->infos->start_time, coder->id);
	pthread_mutex_unlock(&coder->infos->print_mutex);
	usleep(coder->infos->time_to_refactor * 1000);
}

void	*start_simulation(void *argv)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)argv;
	decide_order(coder, &first, &second);
	while (1)
	{
		pthread_mutex_lock(coder->sim_mutex);
		pthread_mutex_unlock(coder->sim_mutex);
		if (!(*(coder->sim_running)))
			break ;
		if (!dongle_take(first, coder))
			break ;
		if (!dongle_take(second, coder))
		{
			dongle_release(first, coder->infos);
			break ;
		}
		coder->last_time_compilation = get_time_in_ms();
		compile_and_relase_dongles(coder, first, second);
		debug_and_refactor(coder);
	}
	return (NULL);
}
