// ./codexion 6 1200 200 200 150 7 100 fifo

// time_to_burnout should be greather than time_to_compile + time_to_debug + time_to_refactor + dongle_cooldown


// here i am gonna fill the shared info struct here and returned as return

#include "codexion.h"

int is_a_valid_number(char* a)
{
	int	i;

	i = 0;
	while (a[i] == '+' || a[i] == '-')
		i++;
	
	while(*a)
	{
		if(a[i] < '0' || a[i] > '9')
		{
			printf("check all arguments to be valid");
			return (0);
		}
		i++;
	}
	return 1;
}

t_shared_info	parsing_codexion(int argc, char **argv)
{
	t_shared_info 	program_info;
	int				i;

	i = 1;
	if(argc < 9)
		printf("you must enter 8 arguments follow: "
			"number_of_coders time_to_burnout time_to_compile "
			"time_to_debug time_to_refactor number_of_compiles_required "
			"dongle_cooldown scheduler");
	else
	{
		while (i < 9)
		{
			if(!is_a_valid_number(argv[i]))
				return;
			i++;
		}

		program_info.number_of_coders = atoi(argv[1]);
		program_info.time_to_burnout = atoi(argv[2]);
		program_info.time_to_compile = atoi(argv[3]);
		program_info.time_to_debug = atoi(argv[4]);
		program_info.time_to_refactor = atoi(argv[5]);
		program_info.number_of_compiles_required = atoi(argv[6]);
		program_info.dongle_cooldown = atoi(argv[7]);
		program_info.scheduler = argv[8];
		if(program_info.scheduler != "fifo" && program_info.scheduler != "edf"
			&& program_info.scheduler != "FIFO" && program_info.scheduler != "EDF")
		{
			printf("THE SCHEDULER must be fifo or edf");
			return;
		}
			
	}

	return program_info;
}
