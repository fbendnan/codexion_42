#include "codexion.h"

void initialize_dongles(int number_of_dongles, t_dongle *dongles)
{
    int i;

    i = 0;
    while (i < number_of_dongles)
    {
        dongles[i].id = i;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        pthread_cond_init(&dongles[i].cond, NULL);
        dongles[i].in_use = 0;
        dongles[i].cooldown_until = 0;
        i++;
    }
}

void initialize_coders(t_shared_info *infos, t_dongle *dongles, t_coder *coders, t_simulation *sim)
{
    int i;

    i = 0;
    while (i < infos->number_of_coders)
    {
        coders[i].id = i + 1;
        coders[i].compiles_done = 0;
        coders[i].state = 0;
        coders[i].last_time_compilation = 0;
        coders[i].right_dongle = &dongles[(i + 1) % infos->number_of_coders];
        coders[i].left_dongle = &dongles[i];
        coders[i].infos = infos;
        coders[i].sim_running = &sim->running;
        coders[i].sim_mutex = &sim->mutex;
        i++;
    }
}

void init_simulation(t_simulation *sim)
{
    sim->running = 1;
    pthread_mutex_init(&sim->mutex, NULL);
}