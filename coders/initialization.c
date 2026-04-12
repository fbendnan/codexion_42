#include "codexion.h"


void	initialize_dongles(int number_of_dongles, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < number_of_dongles)
	{
		dongles[i].id = i;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		i++;
	}
	return;
}

void	initialize_coders(t_shared_info *infos, t_dongle *dongles, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < infos->number_of_coders)
	{
		coders[i].id = i;
		/////////////////////////
		////////////////
		coders[i].compiles_done = 0;
		coders[i].state = 0;
		coders[i].right_dongle = &dongles[(i + 1) % infos->number_of_coders];
		coders[i].left_dongle = &dongles[i];
		coders[i].infos = infos;
		//// add the last time compile
		i++;
	}
}



///////////////////check oncf promo