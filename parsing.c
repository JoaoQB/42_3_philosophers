/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:23:09 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/17 15:16:19 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	validate_input(char *argv)
{
	int	i;

	if (!argv)
		return (1);
	i = 0;
	while (argv[i])
	{
		if (!is_digit(argv[i]))
			return (0);
		if (i > 10)
			return (0);
		i++;
	}
	return (1);
}

int	parse_input(char **argv, t_table *table)
{
	if (!validate_input(argv[1]) || !validate_input(argv[2])
		|| !validate_input(argv[3]) || !validate_input(argv[4])
		|| !validate_input(argv[5]))
		return (0);
	table->seats = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		return (0);
	if (argv[5])
		table->meals_limit = ft_atoi(argv[5]);
	else
		table->meals_limit = -1;
	printf("seats: %d\n", table->seats);
	printf("time_to_die: %d\n", table->time_to_die);
	printf("time_to_eat: %d\n", table->time_to_eat);
	printf("time_to_sleep: %d\n", table->time_to_sleep);
	printf("meals_limit: %d\n", table->meals_limit);
	return (1);
}
