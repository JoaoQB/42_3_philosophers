/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:48:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/21 14:28:02 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// static void	*monitor_philos(t_table *table)

static int	create_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_create(&philo[i].thread_id, NULL, routine, &philo[i]))
		{
			printf("Error creating thread for philo %d.\n", philo[i].index);
			return (-1);
		}
	}
	return (1);
}

static int	join_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_join(philo[i].thread_id, NULL))
		{
			printf("Error joining thread for philo %d.\n", philo[i].index);
			return (-1);
		}
	}
	return (1);
}

int	run_dinner(t_table *table)
{
	if (table->meals_limit == 0)
		return (1);
	// else if (table->seats == 1)
	// 	// TODO
	if (create_threads(table) == -1)
		return (1);
	pthread_mutex_lock(&table->mtx);
	table->start_time = get_time();
	printf("Dinner started at: %ld miliseconds.\n", table->start_time);
	pthread_mutex_unlock(&table->mtx);
	if (table->start_time == -1)
		return (1);
	if (join_threads(table) == -1)
		return (1);
	return (0);
}
