/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 19:38:45 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 12:56:28 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
#include <string.h>

typedef struct s_request_node
{
	int						coder_id;
	long					priority;
	pthread_cond_t			*personal_cond;
	pthread_mutex_t			*personal_mutex;
	struct s_request_node	*next;
}	t_request_node;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	int				in_use;
	long			cooldown_until;
	t_request_node	*wait_queue;
}	t_dongle;

typedef struct s_shared_info
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	long			start_time;
	pthread_mutex_t	print_mutex;
}	t_shared_info;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	long			compiles_done;
	int				state;
	t_dongle		*right_dongle;
	t_dongle		*left_dongle;
	t_shared_info	*infos;
	long			last_time_compilation;
	int				*sim_running;
	pthread_mutex_t	*sim_mutex;
	pthread_cond_t	personal_cond;
	pthread_mutex_t	personal_mutex;
}	t_coder;

typedef struct s_simulation
{
	int				running;
	pthread_mutex_t	mutex;
}	t_simulation;

typedef struct s_monitor
{
	t_coder			*coders;
	t_shared_info	*info;
	t_simulation	*sim;
}	t_monitor;

/* Function prototypes */
void			*start_simulation(void *argv);
int				parsing_codexion(int argc, char **argv,
					t_shared_info *program_info);
void			initialize_dongles(int number_of_dongles, t_dongle *dongles);
void			initialize_coders(t_shared_info *infos, t_dongle *dongles,
					t_coder *coders, t_simulation *sim);
void			create_coders(t_coder *coders);
void			*monitor_routine(void *arg);
int				dongle_take(t_dongle *d, t_coder *coder);
void			dongle_release(t_dongle *d, t_shared_info *info);
void			init_simulation(t_simulation *sim);
long			get_request_priority(t_coder *coder);
void			queue_insert(t_dongle *d, t_request_node *new_node,
					char *scheduler);
t_request_node	*queue_pop(t_dongle *d);
long			get_time_in_ms(void);
void			cleanup(t_dongle *dongles, t_coder *coders,
					t_shared_info *info, t_simulation *sim);
void			display_error(char *error_msg);
int				is_a_valid_number(char *a);
int				is_letter(char a);
void wait_coders_creation(t_coder *coders);

#endif