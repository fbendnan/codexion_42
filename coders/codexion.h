#ifndef CODEXION_H
# define CODEXION_H


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>


typedef struct	s_dongle
{
	int		id;
	pthread_mutex_t		mutex; //a dongle is a mutex
	int					in_use;
	pthread_cond_t		cond;
	long				cooldown_until;
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
	long	start_time;
	pthread_mutex_t		print_mutex; //to loock print

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
	long				last_time_compilation;
	int             *sim_running;            // pointer to sim->running
    pthread_mutex_t *sim_mutex;              // pointer to sim->mutex

	///add last time of finishing compilation
}	t_coder;


typedef struct s_simulation {
    int             running;
    pthread_mutex_t mutex;
} t_simulation;


typedef struct s_monitor_args {
    t_coder         *coders;
    t_shared_info   *info;
    t_simulation    *sim;
} t_monitor_args;






void	*start_simulation(void *argv);
int		parsing_codexion(int argc, char **argv, t_shared_info *program_info);
void	initialize_dongles(int number_of_dongles, t_dongle *dongles);
void initialize_coders(t_shared_info *infos, t_dongle *dongles, t_coder *coders, t_simulation *sim);
void create_threads(t_coder *coders);
void *monitor_routine(void *arg);
int dongle_take(t_dongle *d, t_coder *coder);
void dongle_release(t_dongle *d, t_shared_info *info);
void init_simulation(t_simulation *sim);

long get_time_in_ms(void);


#endif
