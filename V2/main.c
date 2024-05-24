/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:07:35 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 11:36:46 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_args(t_table *table)
{
	int		i;

	printf("Seats: %d\n", table->seats);
	printf("Time to die: %d\n", table->time_to_die);
	printf("Time to eat: %d\n", table->time_to_eat);
	printf("Time to sleep: %d\n", table->time_to_sleep);
	printf("Meals limit: %d\n", table->meals_limit);

	for (i = 0; i < table->seats; i++)
	{
		printf("Philosopher %d was assigned forks %d and %d\n",
			table->phil[i].index,
			table->phil[i].left_fork->index,
			table->phil[i].right_fork->index);
	}
}

static void	print_error(char c)
{
	if (c == 'a')
	{
		printf("Wrong input:\n"
			GRN"Please input i.e. ./philo 6 400 200 200 [10]\n"RST);
	}
	else if (c == 'p')
	{
		printf(RD"Please only use digits;\n"RST);
		printf(RD"Values must be > 60 and < INT_MAX\n"RST);
	}
	return ;
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
	{
		print_error('a');
		return (1);
	}
	else
	{
		if (!parse_input(argv, &table))
		{
			print_error('p');
			return (1);
		}
		if (!init_data(&table))
			return (2);
		print_args(&table);
		if (!init_dinner(&table))
			return (3);
		cleanup(&table);
	}
	return (0);
}
