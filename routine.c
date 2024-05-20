/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:21:58 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/20 11:32:24 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	ft_sleep(philo->time_to_eat);
}

static void	sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_sleep(philo->time_to_sleep);
}

static void	think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (1)
	{
		pthread_mutex_lock(&philo->table_mtx);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->table_mtx);
		if (philo->is_full || philo->ended)
			break;

		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
}
