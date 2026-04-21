//codexion.c


#include "codexion.h"

// 1- create steeps of simulation
// 2- create cond_variables 
// 3- combine the logic


long get_time_in_ms(void)
{
	struct timeval	tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	*start_simulation(void *argv)
{
	t_coder	*coder;
	long	start;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)argv;
	start = get_time_in_ms();

	// decide lock order (anti-deadlock)
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

	while (!coder->state)
	{
		// take first dongle
		pthread_mutex_lock(&first->mutex);
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%li %i has taken a dongle\n",
			get_time_in_ms() - start, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);

		// take second dongle
		pthread_mutex_lock(&second->mutex);
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%li %i has taken a dongle\n",
			get_time_in_ms() - start, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);

		// compiling
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%li %i is compiling\n",
			get_time_in_ms() - start, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);

		usleep(coder->infos->time_to_compile * 1000);
		coder->compiles_done++;

		// debugging
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%li %i is debugging\n",
			get_time_in_ms() - start, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);

		usleep(coder->infos->time_to_debug * 1000);

		// refactoring
		pthread_mutex_lock(&coder->infos->print_mutex);
		printf("%li %i is refactoring\n",
			get_time_in_ms() - start, coder->id);
		pthread_mutex_unlock(&coder->infos->print_mutex);

		usleep(coder->infos->time_to_refactor * 1000);

		// release dongles
		usleep(coder->infos->dongle_cooldown);

		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);

		// stop condition
		if (coder->compiles_done
			>= coder->infos->number_of_compiles_required)
			coder->state = 1;

	}
	return (NULL);
}

void create_threads(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders->infos->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, start_simulation, &coders[i]);
		i++;
	}
	i = 0;
	while (i < coders->infos->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}

	////pthread_mutex_destroy ==>Cleans up the lock at the very end of your program to prevent memory issues.

}
