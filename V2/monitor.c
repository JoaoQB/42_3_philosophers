/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 12:00:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 12:22:49 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&philo->philo_mtx, &philo->full))
		return (false);
	pthread_mutex_lock(&philo->philo_mtx);
	elapsed = get_time() - philo->last_meal_time;
	time_to_die = philo->table->time_to_die;
	pthread_mutex_unlock(&philo->philo_mtx);
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

static bool check_philos(t_table *table, int *served)
{
	int	i;

	i = -1;
	while (++i < table->seats)
	{
		if (philo_died(table->phil + i))
		{
			print_status(table->phil + i, RD"has died"RST);
			set_bool(&table->ended_mtx, &table->ended, true);
			return (true);
		}
		else if (get_bool(&table->phil[i].philo_mtx, &table->phil[i].full))
		{
			(*served)++;
			if (*served == table->seats)
			{
				set_bool(&table->ended_mtx, &table->ended, true);
				return (true);
			}
		}
	}
	return (false);
}

void	*monitor_philos(void *data)
{
	t_table	*table;
	int		served;

	served = 0;
	table = (t_table *)data;
	while (1)
	{
		if (get_bool(&table->ended_mtx, &table->ended))
			return (NULL);
		if (check_philos(table, &served))
			return (NULL);
	}
	return (NULL);
}
