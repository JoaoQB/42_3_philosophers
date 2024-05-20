/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:08:35 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/18 15:46:44 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;
	// DON'T FORGET TO CHANGE MAKEFILE FLAGS!!!! DUMMY
	if (argc == 5 || argc == 6)
	{
		if (!parse_input(argv, &table))
		{
			printf(RED"Please only use digits;\n"RESET);
			printf(RED"Values bigger than 60;\nSmaller than INT_MAX.\n"RESET);
			return (1);
		}
		if (init_dinner(&table))
			return (1);
	}
	else
	{
		printf("Wrong input:\n"
			GREEN"Please input i.e. ./philo 6 400 200 200 [10]\n"RESET);
		return (1);
	}
}
