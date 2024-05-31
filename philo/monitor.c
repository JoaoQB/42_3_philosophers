/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 12:00:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/31 12:54:48 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_full(t_philo *philo)
{
	if (!get_bool(&philo->table->full_mtx, &philo->full))
	{
		pthread_mutex_lock(&philo->philo_mtx);
		if (philo->meals_eaten > 0 && philo->meals_eaten == philo->meals_limit)
		{
			set_bool(&philo->table->full_mtx, &philo->full, true);
			pthread_mutex_unlock(&philo->philo_mtx);
			return (true);
		}
		pthread_mutex_unlock(&philo->philo_mtx);
		return (false);
	}
	else
		return (true);
}

static void	end_sim(t_table *table, int i)
{
	print_status(table->phil + i, RD"died"RST);
	set_bool(&table->ended_mtx, &table->ended, true);
}

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (is_full(philo)
		|| get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return (false);
	pthread_mutex_lock(&philo->philo_mtx);
	if (philo->last_meal_time == 0)
	{
		pthread_mutex_unlock(&philo->philo_mtx);
		return (false);
	}
	elapsed = get_time() - philo->last_meal_time;
	time_to_die = philo->table->time_to_die;
	pthread_mutex_unlock(&philo->philo_mtx);
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

void	*monitor_philos(void *data)
{
	int		i;
	int		served;
	t_table	*table;

	served = 0;
	table = (t_table *)data;
	while (1)
	{
		if (get_bool(&table->ended_mtx, &table->ended))
			return (NULL);
		i = -1;
		while (++i < table->seats)
		{
			if (is_full(&table->phil[i]))
				served++;
			if (served == table->seats)
				return (NULL);
			if (philo_died(table->phil + i))
			{
				end_sim(table, i);
				return (NULL);
			}
		}
	}
	return (NULL);
}
