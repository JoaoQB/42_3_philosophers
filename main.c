/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:08:35 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 11:44:00 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	cleanup(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->seats)
	{
		if (i % 2 == 0)
		{
			if (pthread_mutex_destroy(&(table->phil + i)->left_fork->mtx) != 0)
				return (1);
		}
		else
		{
			if (pthread_mutex_destroy(&(table->phil + i)->right_fork->mtx) != 0)
				return (1);
		}
		if (pthread_mutex_destroy(&(table->phil + i)->philo_mtx) != 0)
			return (1);
	}
	free (table->phil);
	free (table->forks);
	if (pthread_mutex_destroy(&table->mtx) != 0)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc == 5 || argc == 6)
	{
		if (!parse_input(argv, &table))
		{
			printf(RD"Please only use digits;\n"RST);
			printf(RD"Values bigger than 60;\nSmaller than INT_MAX.\n"RST);
			return (1);
		}
		if (init_dinner(&table))
			return (1);
		if (run_dinner(&table))
			return (1);
		if (cleanup(&table))
			return (1);
	}
	else
	{
		printf("Wrong input:\n"
			GRN"Please input i.e. ./philo 6 400 200 200 [10]\n"RST);
		return (1);
	}
}
