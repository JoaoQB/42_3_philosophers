/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:48:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/22 14:39:02 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_one_philo(t_table *table)
{
	t_philo	*philo;

	philo = table->phil;
	if (pthread_create(&philo[0].thread_id, NULL, one_philo, &philo[0]))
	{
		printf("Error creating thread for  one philo.\n");
		return (-1);
	}
	if (pthread_create(&table->monitor, NULL, monitor_philos, table) != 0)
	{
		printf("Error creating monitor thread.\n");
		return (-1);
	}
	return (0);
}

static int	create_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->phil;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_create(&philo[i].thread_id, NULL, routine, &philo[i]))
		{
			printf("Error creating thread for philo %d.\n", philo[i].index);
			return (-1);
		}
	}
	if (pthread_create(&table->monitor, NULL, monitor_philos, table) != 0)
	{
		printf("Error creating monitor thread.\n");
		return (-1);
	}
	return (1);
}

static int	join_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->phil;
	i = -1;
	if (pthread_join(table->monitor, NULL))
	{
		printf("Error joining monitor thread.\n");
		return (-1);
	}
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
	pthread_mutex_lock(&table->mtx);
	table->start_time = get_time();
	if (table->start_time == -1)
		return (1);
	pthread_mutex_unlock(&table->mtx);
	if (table->meals_limit == 0)
		return (1);
	else if (table->seats == 1)
	{
		if (handle_one_philo(table))
			return (1);
	}
	else
	{
		if (create_threads(table) == -1)
			return (1);
	}
	if (join_threads(table) == -1)
		return (1);
	return (0);
}
