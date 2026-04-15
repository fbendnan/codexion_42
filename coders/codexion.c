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
	//// pthread_mutex_lock ==> to lock the dongle
	/////pthread_mutex_unlock ==> to unlock the dongle
	//->convert void type to the type i should work with
	t_coder 	*coder_info;
	long		start;

	coder_info = (t_coder*)argv;
	start = get_time_in_ms();
	

	while (!coder_info->state)
	{
		//when a thread do all the steeps it should sleep by the cond var 
		// when it sleep and othter thread take the dongle i wanna know if it gonna return ===> no it continue from the same place
		// from the first and execute the has taken a dongle or not and also if i am gonna put in 
		// the problem of locking the mutexe 2 times succeced
		if (coder_info->id % 2 == 0)
		{
			pthread_mutex_lock(&coder_info->right_dongle->mutex);
			printf("%li %i has taken a dongle\n", get_time_in_ms() - start, coder_info->id);
			pthread_mutex_lock(&coder_info->left_dongle->mutex);
			// here i am gonna put the cond var to say (if you are pair wait or l3aks 
			// lmhm i am gonna do a cond that make the neighbors work togheter without conflicts)

			printf("%li %i has taken a dongle\n", get_time_in_ms() - start, coder_info->id);
			printf("%li %i is compiling\n", get_time_in_ms() - start, coder_info->id);
			// usleep() works with microseconds
			usleep(coder_info->infos->time_to_compile * 1000);
			coder_info->compiles_done += 1;
			printf("%li %i is debugging\n", get_time_in_ms() - start, coder_info->id);
			usleep(coder_info->infos->time_to_debug * 1000);
			printf("%li %i is refactoring\n", get_time_in_ms() - start, coder_info->id);
			usleep(coder_info->infos->time_to_refactor * 1000);
			if (coder_info->compiles_done == coder_info->infos->number_of_compiles_required)
				coder_info->state = 1;

			pthread_mutex_unlock(&coder_info->right_dongle->mutex);
			pthread_mutex_unlock(&coder_info->left_dongle->mutex);

		}
		if (coder_info->id % 2 != 0)
		{
			pthread_mutex_lock(&coder_info->left_dongle->mutex);
			printf("%li %i has taken a dongle\n", get_time_in_ms() - start, coder_info->id);
			pthread_mutex_lock(&coder_info->right_dongle->mutex);
			// here i am gonna put the cond var to say (if you are pair wait or l3aks 
			// lmhm i am gonna do a cond that make the neighbors work togheter without conflicts)

			printf("%li %i has taken a dongle\n", get_time_in_ms() - start, coder_info->id);
			printf("%li %i is compiling\n", get_time_in_ms() - start, coder_info->id);
			// usleep() works with microseconds
			usleep(coder_info->infos->time_to_compile * 1000);
			coder_info->compiles_done += 1;
			printf("%li %i is debugging\n", get_time_in_ms() - start, coder_info->id);
			usleep(coder_info->infos->time_to_debug * 1000);
			printf("%li %i is refactoring\n", get_time_in_ms() - start, coder_info->id);
			usleep(coder_info->infos->time_to_refactor * 1000);
			if (coder_info->compiles_done == coder_info->infos->number_of_compiles_required)
				coder_info->state = 1;

			pthread_mutex_unlock(&coder_info->left_dongle->mutex);
			pthread_mutex_unlock(&coder_info->right_dongle->mutex);
		}
	}
	return NULL;
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
