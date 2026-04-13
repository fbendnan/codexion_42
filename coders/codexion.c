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
	int			start;

	coder_info = (t_coder*)argv;
	start = get_time_in_ms();
	pthread_mutex_lock(&coder_info->left_dongle->mutex);
	pthread_mutex_lock(&coder_info->right_dongle->mutex);

	while (!coder_info->state)
	{
		//when a thread do all the steeps it should sleep by the cond var 
		// when it sleep and othter thread take the dongle i wanna know if it gonna return 
		// from the first and execute the has taken a dongle or not and also if i am gonna put in 
		// the problem of locking the mutexe 2 times succeced
		printf("%li %i has taken a dongle\n", get_time_in_ms(), coder_info->id);
		printf("%li %i has taken a dongle\n", get_time_in_ms(), coder_info->id);
		printf("%li %i is compiling\n", get_time_in_ms(), coder_info->id);
		coder_info->compiles_done += 1;
		if (coder_info->compiles_done == coder_info->infos->number_of_compiles_required)
			coder_info->state = 1;
		
		
	}
	pthread_mutex_unlock(&coder_info->left_dongle->mutex);
	pthread_mutex_unlock(&coder_info->right_dongle->mutex);
	
	// pthread_mutex_lock()

	return NULL;
}


void create_threads(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders->infos->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, start_simulation, &coders[i]);
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
	////pthread_mutex_destroy ==>Cleans up the lock at the very end of your program to prevent memory issues.

}
