// ./codexion 6 1200 200 200 150 7 100 fifo

// time_to_burnout should be greather than time_to_compile + time_to_debug + time_to_refactor + dongle_cooldown


// here i am gonna fill the shared info struct here and returned as return
/*
1- reject neg numbers and accepte legits
2- reject num > INT_MAX_MIN(we should check the len before)
3- a correct shedular that we should use heap(malloc) 
  -->to make modifications on it to not being in read only memory
4- timestamp > 60ms
5- burnout > .....
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
	while (a[i] == ' ' || (a[i] >= 9 && a[i] <= 13))
		i++;
	if (a[i] == '+')
		i++;
	else if (a[i] == '-')
		return(Display_error("negative numbers are invalid!\n"), 0);
	if (a[i] < '0' || a[i] > '9')
		return(Display_error("numbers are invalid!"), 0);
	
	int start = i;
	while (a[i] >= '0' && a[i] <= '9')
		i++;
	
	if (a[i] != '\0')
        return (Display_error("numbers must contain only digits\n"), 0);

    if (i - start > 10)
        return (Display_error("The value is > than INT_MAX\n"), 0);

	return (1);
}

static inline int is_letter(char a)
{
	return ((a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z'));
}

static int is_a_valid_shedular(char *shedular[])
{
	int res = 0;
	char *shedulars[2] = {"EDF", "FIFO"};
	int shed_len = strlen(*shedular);
	while (**shedular)
	{
		if (!is_letter(**shedular))
			return (0);
		if (**shedular >= 'a' && **shedular <= 'z')
		{
			**shedular -= 32;
		}		
		(*shedular)++;
	}

	*shedular = *(shedular)-shed_len;
	if (strcmp(shedulars[0], *(shedular)) == 0)
		return (1);
	else if (strcmp(shedulars[1], *(shedular)) == 0)
		return (1);
	
	return (0);
}

/*
static int is_a_valid_shedular(char *s)
{
    int i = 0;

    // convert to uppercase safely
    while (s[i])
    {
        if (!is_letter(s[i]))
            return (0);
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
        i++;
    }

    if (strcmp(s, "EDF") == 0 || strcmp(s, "FIFO") == 0)
        return (1);

    return (0);
}
*/

int	parsing_codexion(int argc, char **argv, t_shared_info *program_info)
{
	int	i;

	i = 1;
	if(argc < 9)
		Display_error("you must enter 8 arguments follow: "
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
			
		program_info->number_of_coders = atoi(argv[1]);
		program_info->time_to_burnout = atoi(argv[2]);

		program_info->time_to_compile = atoi(argv[3]);
		program_info->time_to_debug = atoi(argv[4]);
		program_info->time_to_refactor = atoi(argv[5]);
		program_info->number_of_compiles_required = atoi(argv[6]);
		program_info->dongle_cooldown = atoi(argv[7]);
		program_info->scheduler = malloc(strlen(argv[8]) + 1);
		strcpy(program_info->scheduler, argv[8]);

		if (!is_a_valid_shedular(&(program_info->scheduler)))
			return ((program_info->scheduler = NULL),
			Display_error("Enter a valid schedular(FIFO/EDF)\n"), 0);
	}
	return (1);
}


/*
int parsing_codexion(int argc, char **argv, t_shared_info *program_info)
{
    int i = 1;

    if (argc != 9)
        return (Display_error("you must enter exactly 8 arguments\n"), 0);

    while (i < 8)
    {
        if (!is_a_valid_number(argv[i]))
            return (0);
        i++;
    }

    program_info->number_of_coders = atoi(argv[1]);
    program_info->time_to_burnout = atoi(argv[2]);
    program_info->time_to_compile = atoi(argv[3]);
    program_info->time_to_debug = atoi(argv[4]);
    program_info->time_to_refactor = atoi(argv[5]);
    program_info->number_of_compiles_required = atoi(argv[6]);
    program_info->dongle_cooldown = atoi(argv[7]);

    program_info->scheduler = malloc(strlen(argv[8]) + 1);
    if (!program_info->scheduler)
        return (0);

    strcpy(program_info->scheduler, argv[8]);

    if (!is_a_valid_shedular(program_info->scheduler))
    {
        free(program_info->scheduler);
        return (Display_error("Enter a valid scheduler (FIFO/EDF)\n"), 0);
    }

    return (1);
}
*/