// dongle.c

#include "codexion.h"


int dongle_take(t_dongle *d, t_coder *coder)
{
    struct timespec ts;
    long now;
    long target;

    pthread_mutex_lock(&d->mutex);
    while (d->in_use || (now = get_time_in_ms()) < d->cooldown_until)
    {
        if (d->in_use)
        {
            // Dongle is in use; wait indefinitely until released
            pthread_cond_wait(&d->cond, &d->mutex);
        }
        else
        {
            // Dongle is free but in cooldown; wait until cooldown expires
            target = d->cooldown_until;
            ts.tv_sec = target / 1000;
            ts.tv_nsec = (target % 1000) * 1000000;
            pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
            now = get_time_in_ms();
        }
        // Re-check condition after waking
    }
    d->in_use = 1;
    pthread_mutex_unlock(&d->mutex);
    pthread_mutex_lock(&coder->infos->print_mutex);
    printf("%ld %d has taken a dongle\n", get_time_in_ms() - coder->infos->start_time, coder->id);
    pthread_mutex_unlock(&coder->infos->print_mutex);
    return (1);
}

void dongle_release(t_dongle *d, t_shared_info *info)
{
    pthread_mutex_lock(&d->mutex);
    d->in_use = 0;
    d->cooldown_until = get_time_in_ms() + info->dongle_cooldown;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}
