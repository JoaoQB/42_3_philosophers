/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:08:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/22 11:55:34 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&philo->philo_mtx, &philo->is_full))
		return (false);
	pthread_mutex_lock(&philo->philo_mtx);
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
		if (get_bool(&table->mtx, &table->ended))
			return (NULL);
		i = -1;
		while (++i < table->seats)
		{
			if (get_bool(&(table->philos + i)->philo_mtx, &(table->philos + i)->is_full))
				served++;
			if (served == table->seats)
				return (NULL);
			if (philo_died(table->philos + i))
			{
				print_status(table->philos + i, RD"has died"RST);
				set_bool(&table->mtx, &table->ended, true);
				return (NULL);
			}
		}
	}
	return (NULL);
}
