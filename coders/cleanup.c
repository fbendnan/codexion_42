// gcc -Wall -Wextra -Werror -pthread *.c -o codexion
// ./codexion 2 2000 100 100 100 2 50 fifo

//cleanup.c
#include "codexion.h"

void cleanup(t_dongle *dongles, t_coder *coders, t_shared_info *info, t_simulation *sim)
{
    int             i;
    t_request_node  *tmp;
    t_request_node  *next;

    i = 0;
    while (i < info->number_of_coders)
    {
        // Destroy dongle mutex
        pthread_mutex_destroy(&dongles[i].mutex);

        // Destroy per-coder personal condition variable and mutex
        pthread_cond_destroy(&coders[i].personal_cond);
        pthread_mutex_destroy(&coders[i].personal_mutex);

        // Free any remaining request nodes in the dongle's wait queue
        tmp = dongles[i].wait_queue;
        while (tmp)
        {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }

        // Destroy the dongle's condition variable (if you kept it)
        // pthread_cond_destroy(&dongles[i].cond);

        i++;
    }

    // Destroy print mutex
    pthread_mutex_destroy(&info->print_mutex);

    // Destroy simulation mutex
    pthread_mutex_destroy(&sim->mutex);

    // Free the scheduler string
    free(info->scheduler);

    // Free the arrays
    free(dongles);
    free(coders);
}