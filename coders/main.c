
// // //main.c

// #include "codexion.h"

// // void *do_compilation(void *comp)
// // {
// //     // dongle1 = 
// //     // pthread_mutex_lock(&dongle1)

// // }

// int main(int argc, char **argv)
// {
//     t_shared_info   info;
//     t_coder         *coders;
//     t_dongle        *dongles;
//     t_simulation    sim;
//     t_monitor_args  mon_args;
//     pthread_t       monitor_thread;
//     int             i;


//     if (!parsing_codexion(argc, argv, &info))
//         return (1);

//     // Set start time for relative logging
//     info.start_time = get_time_in_ms();
//     pthread_mutex_init(&info.print_mutex, NULL);

//     // Allocate and initialize dongles
//     dongles = malloc(sizeof(t_dongle) * info.number_of_coders);
//     if (!dongles)
//     {
//         write(2, "malloc failed\n", 14);
//         return (1);
//     }
//     initialize_dongles(info.number_of_coders, dongles);

//     // Initialize simulation state
//     init_simulation(&sim);

//     // Allocate and initialize coders
//     coders = malloc(sizeof(t_coder) * info.number_of_coders);
//     if (!coders)
//     {
//         write(2, "malloc failed\n", 14);
//         free(dongles);
//         return (1);
//     }
//     initialize_coders(&info, dongles, coders, &sim);

//     // Create coder threads
//     i = 0;
//     while (i < info.number_of_coders)
//     {
//         pthread_create(&coders[i].thread_id, NULL, start_simulation, &coders[i]);
//         i++;
//     }

//     // Create monitor thread
//     mon_args.coders = coders;
//     mon_args.info = &info;
//     mon_args.sim = &sim;
//     pthread_create(&monitor_thread, NULL, monitor_routine, &mon_args);

//     // Join coder threads
//     i = 0;
//     while (i < info.number_of_coders)
//     {
//         pthread_join(coders[i].thread_id, NULL);
//         i++;
//     }

//     // Join monitor thread
//     pthread_join(monitor_thread, NULL);

//     // Cleanup
//     i = 0;
//     while (i < info.number_of_coders)
//     {
//         pthread_mutex_destroy(&dongles[i].mutex);
//         pthread_cond_destroy(&dongles[i].cond);
//         i++;
//     }
//     pthread_mutex_destroy(&info.print_mutex);
//     pthread_mutex_destroy(&sim.mutex);
//     free(info.scheduler);
//     free(dongles);
//     free(coders);

//     return (0);


//     // if (parsing_codexion(argc, argv, &info))
//     // {
//     //     dongles = malloc(info.number_of_coders * sizeof(t_dongle) + 1);
//     //     initialize_dongles(info.number_of_coders, dongles);
//     //     coders = malloc(info.number_of_coders * sizeof(t_coder) + 1);
//     //     initialize_coders(&info, dongles, coders);
    
//     // 	pthread_mutex_init(&info.print_mutex, NULL);

//     //     create_threads(coders);



//     //     free(dongles);
//     //     free(coders);
//     // }

// }


#include "codexion.h"

int main(int argc, char **argv)
{
    t_shared_info   info;
    t_dongle        *dongles;
    t_coder         *coders;
    t_simulation    sim;
    t_monitor_args  mon_args;
    pthread_t       monitor_thread;
    int             i;

    // 1. Parse and validate arguments
    if (!parsing_codexion(argc, argv, &info))
        return (1);

    // 2. Record simulation start time for relative logging
    info.start_time = get_time_in_ms();
    pthread_mutex_init(&info.print_mutex, NULL);

    // 3. Allocate and initialize dongles (with wait queues)
    dongles = malloc(sizeof(t_dongle) * info.number_of_coders);
    if (!dongles)
    {
        write(2, "Error: malloc failed\n", 21);
        return (1);
    }
    initialize_dongles(info.number_of_coders, dongles);

    // 4. Initialize simulation state (running flag + mutex)
    init_simulation(&sim);

    // 5. Allocate and initialize coders (with personal cond/mutex)
    coders = malloc(sizeof(t_coder) * info.number_of_coders);
    if (!coders)
    {
        write(2, "Error: malloc failed\n", 21);
        free(dongles);
        return (1);
    }
    initialize_coders(&info, dongles, coders, &sim);

    // 6. Create coder threads
    i = 0;
    while (i < info.number_of_coders)
    {
        pthread_create(&coders[i].thread_id, NULL, start_simulation, &coders[i]);
        i++;
    }

    // 7. Create monitor thread
    mon_args.coders = coders;
    mon_args.info = &info;
    mon_args.sim = &sim;
    pthread_create(&monitor_thread, NULL, monitor_routine, &mon_args);

    // 8. Wait for all coder threads to finish
    i = 0;
    while (i < info.number_of_coders)
    {
        pthread_join(coders[i].thread_id, NULL);
        i++;
    }

    // 9. Wait for monitor thread to finish
    pthread_join(monitor_thread, NULL);

    // 10. Cleanup all resources
    cleanup(dongles, coders, &info, &sim);

    return (0);
}