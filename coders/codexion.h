#ifndef CODEXION_H
# define CODEXION_H


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



//dongle struct

typedef struct	s_dongle
{
	int		id;
	pthread_mutex_t		mutex; //a dongle is a mutex
}	t_dongle;

//shared info struct
typedef struct	s_shared_info
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char*	scheduler;
}	t_shared_info;

//coder struct
typedef struct	s_coder
{
	int				id;
	pthread_t 		thread_id; //a coder is a thread
	long			compiles_done;
	int				state;
	t_dongle		*right_dongle;
	t_dongle		*left_dongle;
	t_shared_info 	*infos;
	int				las_time_compilation;
	///add last time of finishing compilation
}	t_coder;

void	*start_simulation(void *argv);
int		parsing_codexion(int argc, char **argv, t_shared_info *program_info);
void	initialize_dongles(int number_of_dongles, t_dongle *dongles);
void	initialize_coders(t_shared_info *infos, t_dongle *dongles, t_coder *coders);
void create_threads(t_coder *coders);

#endif
