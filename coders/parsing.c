/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:30:25 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/24 13:30:26 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./codexion 6 1200 200 200 150 7 100 fifo

#include "codexion.h"

static int is_a_valid_shedular(char *s)
{
    int i = 0;

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

int parsing_codexion(int argc, char **argv, t_shared_info *program_info)
{
    int i = 1;

    if (argc != 9)
        return (display_error("you must enter exactly 8 arguments\n"), 0);

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

    if (program_info->number_of_coders < 2)
        return (display_error("At least 2 coders are required\n"), 0);

    if (!is_a_valid_shedular(program_info->scheduler))
    {
        free(program_info->scheduler);
        return (display_error("Enter a valid scheduler (FIFO/EDF)\n"), 0);
    }
    return (1);
}