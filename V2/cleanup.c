/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 18:23:27 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 11:30:22 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_mtx(int i, t_table *table, char c)
{
	int	j;

	j = -1;
	if (c == 'f')
	{
		while (++j < i)
			pthread_mutex_destroy(&table->forks[j].mtx);
	}
	else if (c == 'p')
	{
		while (++j < i)
			pthread_mutex_destroy(&table->phil[j].philo_mtx);
	}
	else if (c == 'a')
	{
		while (++j < i)
			pthread_mutex_destroy(&table->forks[j].mtx);
		j = -1;
		while (++j < i)
			pthread_mutex_destroy(&table->phil[j].philo_mtx);
	}
	return ;
}

void	free_data(t_table *table)
{
	if (table->phil)
		free (table->phil);
	if (table->forks)
		free (table->forks);
}

void	cleanup(t_table *table)
{
	free_mtx(table->seats, table, 'a');
	pthread_mutex_destroy(&table->ended_mtx);
	pthread_mutex_destroy(&table->write_mtx);
	pthread_mutex_destroy(&table->monitor_mtx);
	if (table->phil)
		free (table->phil);
	if (table->forks)
		free (table->forks);
}
