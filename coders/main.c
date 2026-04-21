
//main.c

#include "codexion.h"

// void *do_compilation(void *comp)
// {
//     // dongle1 = 
//     // pthread_mutex_lock(&dongle1)

// }

int main(int argc, char **argv)
{
    t_shared_info   info;
    t_coder         *coders;
    t_dongle        *dongles;

    //------------------testing the parsing------------------------

    // int h = parsing_codexion(argc, argv, &info);
    // printf("number of coders = %i \n", info.number_of_coders);
    // printf("time_to_burnout = %i \n", info.time_to_burnout);
    // printf("time_to_compile = %i \n", info.time_to_compile);
    // printf("time_to_debug = %i \n", info.time_to_debug);
    // printf("time_to_refactor = %i \n", info.time_to_refactor);
    // printf("number_of_compiles_required = %i \n", info.number_of_compiles_required);
    // printf("scheduler = %s \n", info.scheduler);


    if (parsing_codexion(argc, argv, &info))
    {
        dongles = malloc(info.number_of_coders * sizeof(t_dongle) + 1);
        initialize_dongles(info.number_of_coders, dongles);
        coders = malloc(info.number_of_coders * sizeof(t_coder) + 1);
        initialize_coders(&info, dongles, coders);
    	pthread_mutex_init(&info.print_mutex, NULL);

        create_threads(coders);



        free(dongles);
        free(coders);
    }

    return (0);
}
