/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:52:05 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/30 19:01:52 by jqueijo-         ###   ########.fr       */
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
		if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
			break ;
		ft_sleep(5, philo->table);
	}
	return (NULL);
}

static void	think(t_philo *philo)
{
	long	last_ate_elapsed;
	long	time_to_think;

	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	print_status(philo, "is thinking");
	last_ate_elapsed = get_time() - philo->last_meal_time;
	time_to_think = last_ate_elapsed / 2;
	if (time_to_think + last_ate_elapsed > philo->table->time_to_die)
		time_to_think = 1;
	ft_sleep(time_to_think, philo->table);
}

static void	rest(t_philo *philo)
{
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	print_status(philo, "is sleeping");
	ft_sleep(philo->table->time_to_sleep, philo->table);
}

static void	eat(t_philo *philo)
{
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	pthread_mutex_lock(&philo->left_fork->mtx);
	print_status(philo, "has taken the first fork");
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
	{
		pthread_mutex_unlock(&philo->left_fork->mtx);
		return ;
	}
	pthread_mutex_lock(&philo->right_fork->mtx);
	print_status(philo, "has taken the second fork");
	pthread_mutex_lock(&philo->philo_mtx);
	philo->meals_eaten++;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	print_status(philo, GRN"is eating"RST);
	ft_sleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_unlock(&philo->right_fork->mtx);
	pthread_mutex_unlock(&philo->left_fork->mtx);
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
		if (is_full(philo))
			break ;
		if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
			break ;
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
}
