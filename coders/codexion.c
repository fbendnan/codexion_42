//codexion.c


#include "codexion.h"

void *start_simulation(void *argv)
{
    t_coder  *coder;
    // long     start;
    t_dongle *first;
    t_dongle *second;
    int      keep_running;

    coder = (t_coder *)argv;
    // start = get_time_in_ms();

    // Decide lock order to prevent deadlock
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

    while (1)
    {
        // --- Check global stop flag (set by monitor thread) ---
        pthread_mutex_lock(coder->sim_mutex);
        keep_running = *(coder->sim_running);
        pthread_mutex_unlock(coder->sim_mutex);
        if (!keep_running)
            break;

        // --- Try to acquire both dongles (with proper cooldown) ---
        if (!dongle_take(first, coder))
            break; // Should never fail, but safety
        if (!dongle_take(second, coder))
        {
            dongle_release(first, coder->infos);
            break;
        }

        // --- Record compile start time for burnout detection ---
        coder->last_time_compilation = get_time_in_ms();

        // --- Compile ---
        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%li %i is compiling\n",
               get_time_in_ms() - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);

        usleep(coder->infos->time_to_compile * 1000);
        coder->compiles_done++;

        // --- Release dongles (starts cooldown on each) ---
        dongle_release(first, coder->infos);
        dongle_release(second, coder->infos);

        // --- Debug ---
        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%li %i is debugging\n",
               get_time_in_ms() - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);

        usleep(coder->infos->time_to_debug * 1000);

        // --- Refactor ---
        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%li %i is refactoring\n",
               get_time_in_ms() - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);

        usleep(coder->infos->time_to_refactor * 1000);

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



// void	*start_simulation(void *argv)
// {
// 	t_coder	*coder;
// 	long	start;
// 	t_dongle	*first;
// 	t_dongle	*second;

// 	coder = (t_coder *)argv;
// 	start = get_time_in_ms();

// 	// decide lock order (anti-deadlock)
// 	if (coder->id % 2 == 0)
// 	{
// 		first = coder->right_dongle;
// 		second = coder->left_dongle;
// 	}
// 	else
// 	{
// 		first = coder->left_dongle;
// 		second = coder->right_dongle;
// 	}

// 	while (!coder->state)
// 	{
// 		coder->last_time_compilation = get_time_in_ms();
// 		if (coder->infos->time_to_burnout < get_time_in_ms() - coder->last_time_compilation)
// 		{
// 			printf("%li %i burned out", get_time_in_ms() - start, coder->id);
// 			return;
// 		}
// 		// take first dongle
// 		pthread_mutex_lock(&first->mutex);
// 		pthread_mutex_lock(&coder->infos->print_mutex);
// 		printf("%li %i has taken a dongle\n",
// 			get_time_in_ms() - start, coder->id);
// 		pthread_mutex_unlock(&coder->infos->print_mutex);

// 		// take second dongle
// 		pthread_mutex_lock(&second->mutex);
// 		pthread_mutex_lock(&coder->infos->print_mutex);
// 		printf("%li %i has taken a dongle\n",
// 			get_time_in_ms() - start, coder->id);
// 		pthread_mutex_unlock(&coder->infos->print_mutex);

// 		// compiling
// 		pthread_mutex_lock(&coder->infos->print_mutex);
// 		printf("%li %i is compiling\n",
// 			get_time_in_ms() - start, coder->id);
// 		pthread_mutex_unlock(&coder->infos->print_mutex);

// 		usleep(coder->infos->time_to_compile * 1000);
// 		coder->compiles_done++;

// 		// debugging
// 		pthread_mutex_lock(&coder->infos->print_mutex);
// 		printf("%li %i is debugging\n",
// 			get_time_in_ms() - start, coder->id);
// 		pthread_mutex_unlock(&coder->infos->print_mutex);

// 		usleep(coder->infos->time_to_debug * 1000);

// 		// refactoring
// 		pthread_mutex_lock(&coder->infos->print_mutex);
// 		printf("%li %i is refactoring\n",
// 			get_time_in_ms() - start, coder->id);
// 		pthread_mutex_unlock(&coder->infos->print_mutex);

// 		usleep(coder->infos->time_to_refactor * 1000);

// 		// release dongles

// 		pthread_mutex_unlock(&first->mutex);
// 		pthread_mutex_unlock(&second->mutex);

// 		usleep(coder->infos->dongle_cooldown);
// 		// stop condition
// 		if (coder->compiles_done
// 			>= coder->infos->number_of_compiles_required)
// 			coder->state = 1;

// 	}
// 	return (NULL);
// }