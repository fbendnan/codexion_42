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
	int	i;

	i = 0;
	if (a[i] == '+')
		i++;
	else if (a[i] == '-')
		return(Display_error("negative numbers are invalid!"), 0);

	if (a[i] != '+' && a[i] != '-')
		return(Display_error("negative numbers are invalid!"), 0);
	while(a[i])
	{
		printf("a[%i] = %i\n", i, a[i]);
		if(a[i] < '0' && a[i] > '9')
			return (Display_error("check all arguments to be valid\n"), 0);
		i++;
	}
	return (1);
}


int is_a_valid_shedular(char *shedular[])
{
	int i = 0;
	char *shedulars[2] = {"EDF", "FIFO"};
	while (shedular[0][i])
	{
		if ((shedular[0][i] <= 'A' && shedular[0][i] >= 'Z') 
			&& (shedular[0][i] <= 'a' && shedular[0][i] >= 'z'))
			return (0);
		if (shedular[0][i]<= 'a' && shedular[0][i] >= 'z')
			shedular[0][i] += 32;
		i++;
	}
	printf("strcmp(shedulars[0], *shedular) = %i\n", strcmp("GHJ", *shedular));
	printf("strcmp(shedulars[1], *shedular) = %i\n", strcmp(shedulars[1], *shedular));
	
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
	char *shedular = "edf";
	const char *N = "933534637824738344";

	printf("the shedular fun = %i", is_a_valid_shedular(&shedular));
	printf("\nthe int max greather = %i\n", atoi(N));
}