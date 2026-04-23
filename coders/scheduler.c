#include "codexion.h"

long	get_request_priority(t_coder *coder)
{
    if (strcmp(coder->infos->scheduler, "FIFO") == 0)
        return (get_time_in_ms());  // arrival time
    else
        return (coder->last_time_compilation + coder->infos->time_to_burnout);
}

t_request_node *queue_pop(t_dongle *d)
{
    t_request_node *first;

    first = d->wait_queue;
    if (first)
        d->wait_queue = first->next;
    return (first);
}


void queue_insert(t_dongle *d, t_request_node *new_node, char *scheduler)
{
    t_request_node *curr;
    t_request_node *prev;

    new_node->next = NULL;
    
    // Empty queue -> insert as head
    if (!d->wait_queue)
    {
        d->wait_queue = new_node;
        return;
    }
    
    if (strcmp(scheduler, "FIFO") == 0)
    {
        // FIFO: always insert at the end (priority = arrival time, always increasing)
        curr = d->wait_queue;
        while (curr->next)
            curr = curr->next;
        curr->next = new_node;
    }
    else  // EDF
    {
        // EDF: insert sorted by priority (smallest deadline first)
        prev = NULL;
        curr = d->wait_queue;
        while (curr && curr->priority <= new_node->priority)
        {
            prev = curr;
            curr = curr->next;
        }
        if (!prev)  // insert at head
        {
            new_node->next = d->wait_queue;
            d->wait_queue = new_node;
        }
        else  // insert in middle or end
        {
            prev->next = new_node;
            new_node->next = curr;
        }
    }
}
