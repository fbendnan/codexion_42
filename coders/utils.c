/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbendnan <fbendnan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 19:25:58 by fbendnan          #+#    #+#             */
/*   Updated: 2026/04/23 19:28:22 by fbendnan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	display_error(char *error_msg)
{
	int	error_len;

	error_len = strlen(error_msg);
	write(2, error_msg, error_len);
}

int	is_a_valid_number(char *a)
{
	int	i;
	int	start;

	i = 0;
	while (a[i] == ' ' || (a[i] >= 9 && a[i] <= 13))
		i++;
	if (a[i] == '+')
		i++;
	else if (a[i] == '-')
		return (display_error("negative numbers are invalid!\n"), 0);
	if (a[i] < '0' || a[i] > '9')
		return (display_error("numbers are invalid!\n"), 0);
	start = i;
	while (a[i] >= '0' && a[i] <= '9')
		i++;
	if (a[i] != '\0')
		return (display_error("numbers must contain only digits\n"), 0);
	if (i - start > 10)
		return (display_error("The value is > than INT_MAX\n"), 0);
	return (1);
}

int	is_letter(char a)
{
	return ((a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z'));
}
