/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:08:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/22 10:22:42 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&philo->philo_mtx, &philo->is_full))
		return (false);
	pthread_mutex_lock(&philo->table->mtx);
	elapsed = get_time() - philo->last_meal_time;
	time_to_die = philo->table->time_to_die;
	pthread_mutex_unlock(&philo->table->mtx);
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

void	*monitor_philos(void *data)
{
	int		i;
	t_table	*table;

	table = (t_table *)data;
	while (!get_bool(&table->mtx, &table->ended))
	{
		i = -1;
		while (++i < table->seats && !get_bool(&table->mtx, &table->ended))
		{
			if (philo_died(table->philos + i))
			{
				print_status(table->philos + i, RD"has died"RST);
				set_bool(&table->philos->philo_mtx, &table->ended, true);
			}
		}
	}
	return (NULL);
}
