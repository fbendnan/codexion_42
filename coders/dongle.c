#include "codexion.h"

int dongle_take(t_dongle *d, t_coder *coder)
{
    t_request_node *req_node;
    long now;

    pthread_mutex_lock(&d->mutex);

    now = get_time_in_ms();
    // If the dongle is free and cooldown is over, take it immediately
    if (!d->in_use && now >= d->cooldown_until)
    {
        d->in_use = 1;
        pthread_mutex_unlock(&d->mutex);
        // Log acquisition
        pthread_mutex_lock(&coder->infos->print_mutex);
        printf("%ld %d has taken a dongle\n",
               now - coder->infos->start_time, coder->id);
        pthread_mutex_unlock(&coder->infos->print_mutex);
        return (1);
    }

    // Dongle is not available – we must queue up
    req_node = malloc(sizeof(t_request_node));
    if (!req_node)
    {
        pthread_mutex_unlock(&d->mutex);
        return (0);
    }

    req_node->coder_id = coder->id;
    req_node->priority = get_request_priority(coder);
    req_node->personal_cond = &coder->personal_cond;
    req_node->personal_mutex = &coder->personal_mutex;
    req_node->next = NULL;

    queue_insert(d, req_node, coder->infos->scheduler);

    // Wait on personal condition (release dongle mutex before wait!)
    pthread_mutex_lock(&coder->personal_mutex);
    pthread_mutex_unlock(&d->mutex);
    pthread_cond_wait(&coder->personal_cond, &coder->personal_mutex);
    pthread_mutex_unlock(&coder->personal_mutex);

    // We've been chosen. Now claim the dongle.
    pthread_mutex_lock(&d->mutex);
    // Wait for cooldown if it's still active (should be handled, but safe)
    while ((now = get_time_in_ms()) < d->cooldown_until)
    {
        pthread_mutex_unlock(&d->mutex);
        usleep(100);
        pthread_mutex_lock(&d->mutex);
    }
    d->in_use = 1;
    pthread_mutex_unlock(&d->mutex);

    // Log acquisition
    pthread_mutex_lock(&coder->infos->print_mutex);
    printf("%ld %d has taken a dongle\n",
           get_time_in_ms() - coder->infos->start_time, coder->id);
    pthread_mutex_unlock(&coder->infos->print_mutex);

    return (1);
}

void dongle_release(t_dongle *d, t_shared_info *info)
{
    t_request_node *next_node;
    
    pthread_mutex_lock(&d->mutex);
    
    d->in_use = 0;
    d->cooldown_until = get_time_in_ms() + info->dongle_cooldown;
    
    // Pop the next waiter (if any)
    next_node = queue_pop(d);
    
    pthread_mutex_unlock(&d->mutex);
    
    // Signal the chosen coder
    if (next_node)
    {
        pthread_mutex_lock(next_node->personal_mutex);
        pthread_cond_signal(next_node->personal_cond);
        pthread_mutex_unlock(next_node->personal_mutex);
        free(next_node);  // free the node after signaling
    }
}
