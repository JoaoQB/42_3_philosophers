/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:21:58 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/22 08:55:05 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->philo_mtx);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	print_status(philo, "has taken the first fork");
	while (1)
	{
		if (get_bool(&philo->table->mtx, &philo->table->ended))
			break ;
		ft_sleep(5, philo->table);
	}
	return (NULL);
}

static void	eat(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->left_fork->mtx) != 0)
		write(2, "left fork failed", 17);
	print_status(philo, "has taken the first fork");
	if (pthread_mutex_lock(&philo->right_fork->mtx) != 0)
		write(2, "right fork failed", 18);
	print_status(philo, "has taken the second fork");
	if (pthread_mutex_lock(&philo->philo_mtx) != 0)
		write(2, "last meal failed", 17);
	philo->meals_eaten++;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	print_status(philo, GREEN"is eating"RESET);
	ft_sleep(philo->table->time_to_eat, philo->table);
	if (philo->meals_limit > 0
		&& philo->meals_eaten == philo->meals_limit)
		set_bool(&philo->philo_mtx, &philo->is_full, true);
	pthread_mutex_unlock(&philo->left_fork->mtx);
	pthread_mutex_unlock(&philo->right_fork->mtx);
}

static void	rest(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_sleep(philo->table->time_to_sleep, philo->table);
}

static void	think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->philo_mtx);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	while (1)
	{
		if (get_bool(&philo->table->mtx, &philo->is_full))
			break ;
		if (get_bool(&philo->table->mtx, &philo->table->ended))
			break ;
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
}
