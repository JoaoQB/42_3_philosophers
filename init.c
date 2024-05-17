/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:29:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/17 16:17:31 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	data_init(t_table *table)
{
	int	i;

	i = 0;
	while(i < table->seats)
	{

	}
}

int	start_dinner(t_table *table)
{
	table->ended = false;
	table->philosophers = malloc(sizeof(t_philo) * table->seats);
	if (!table->philosophers)
		return NULL;
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free(table->philosophers);
		return NULL;
	}
	data_init(table);
}
