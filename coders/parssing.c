// ./codexion 6 1200 200 200 150 7 100 fifo

// time_to_burnout should be greather than time_to_compile + time_to_debug + time_to_refactor + dongle_cooldown


// here i am gonna fill the shared info struct here and returned as return
/*
1- reject neg numbers
2- reject num > INT_MAX_MIN(we should check the lent before)
3- 
*/

#include "codexion.h"

static void Display_error(char* error_msg)
{
	int error_len = strlen(error_msg);
	write(2, error_msg , error_len);

}

static int is_a_valid_number(char* a)
{
	if (*a == '+')
		a++;
	else if (*a == '-')
		return(Display_error("negative numbers are invalid!"), 0);
	else if (*a < '0' || *a > '9')
		return(Display_error("numbers are invalid!"), 0);
	return (1);
}

static int is_letter(char a)
{
	return ((a <= 'A' && a >= 'Z') || (a <= 'a' && a >= 'z'));
}
static int is_a_valid_shedular(char *shedular[])
{
	int i = 0;
	int res = 0;
	char *shedulars[2] = {"EDF", "FIFO"};
	int shed_len = strlen(*shedular);
	while (**shedular)
	{
		// if (!is_letter(shedular[0][i]))
		// 	return (0);
		if (**shedular >= 'a' && **shedular <= 'z')
		{
			**shedular -= 32;
			printf("dkheeelt\n");
		}
		printf("\nthe shedular[0][%i] is %c", i, *shedular[i]);
		
		(*shedular)++;
	}

	printf("\nthe shedular is %s\n", *(shedular)-shed_len);
	*shedular = *(shedular)-shed_len;
	if (strcmp(shedulars[0], *(shedular)) == 0)
		return (1);
	else if (strcmp(shedulars[1], *(shedular)) == 0)
		return (1);
	
	return (0);
}

int	parsing_codexion(int argc, char **argv, t_shared_info *program_info)
{
	int				i;

	i = 1;
	if(argc < 9)
		printf("you must enter 8 arguments follow: "
			"number_of_coders time_to_burnout time_to_compile "
			"time_to_debug time_to_refactor number_of_compiles_required "
			"dongle_cooldown scheduler");
	else
	{
		while (i < 8)
		{
			if(!is_a_valid_number(argv[i]))
				return(0);
			i++;
		}
		if (is_a_valid_shedular(&argv[8]))
		{
			program_info->number_of_coders = atoi(argv[1]);
			program_info->time_to_burnout = atoi(argv[2]);
			program_info->time_to_compile = atoi(argv[3]);
			program_info->time_to_debug = atoi(argv[4]);
			program_info->time_to_refactor = atoi(argv[5]);
			program_info->number_of_compiles_required = atoi(argv[6]);
			program_info->dongle_cooldown = atoi(argv[7]);
			program_info->scheduler = argv[8];
		}
		
			
	}
	return (1);
}

int main()
{
	char *shedular = malloc(5);
	strcpy(shedular, "edf");

	printf("the shedular fun = %i", is_a_valid_shedular(&shedular));
	free(shedular);
}