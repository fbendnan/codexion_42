#include "codexion.h"




// void *do_compilation(void *comp)
// {
//     // dongle1 = 
//     // pthread_mutex_lock(&dongle1)

// }

int main(int argc, char **argv)
{
    t_shared_info   *info;
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


    if (parsing_codexion(argc, argv, info))
    {
        dongles = malloc(info->number_of_coders * sizeof(t_dongle) + 1);
        
    }
    

    // pthread_t coder1;
    // pthread_mutex_t dongle1;
    // pthread_mutex_t dongle2;

    // pthread_mutex_t dongles[2] = dongle1, dongle2;
    
    // pthread_create(&coder1, NULL, do_compilation, &dongles);
    // pthread_join(coder1, NULL);

    return (0);
}
