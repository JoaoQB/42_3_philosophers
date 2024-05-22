/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:08:35 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/22 10:22:32 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	}
	else
	{
		printf("Wrong input:\n"
			GRN"Please input i.e. ./philo 6 400 200 200 [10]\n"RST);
		return (1);
	}
}
