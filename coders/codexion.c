#include "codexion.h"


void	*start_simulation(void *argv)
{
	//// pthread_mutex_lock ==> to lock the dongle
	/////pthread_mutex_unlock ==> to unlock the dongle
}


void create_threads(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders->infos->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, start_simulation, &coders[i]);
		pthread_join(coders[i].id, NULL);
		i++;
	}
	////pthread_mutex_destroy ==>Cleans up the lock at the very end of your program to prevent memory issues.

	return;
}
