/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:56:19 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/31 12:44:57 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->table->seats;
	if (philo->index % 2 == 0)
	{
		philo->left_fork = &forks[position];
		philo->right_fork = &forks[(position + 1) % philo_nbr];
	}
	else
	{
		philo->left_fork = &forks[(position + 1) % philo_nbr];
		philo->right_fork = &forks[position];
	}
}

static bool	init_mtx(t_table *table)
{
	if (pthread_mutex_init(&table->ended_mtx, NULL))
	{
		return (false);
	}
	else if (pthread_mutex_init(&table->write_mtx, NULL))
	{
		free_mtx(table->seats, table, 'a');
		pthread_mutex_destroy(&table->ended_mtx);
		free_data(table);
		return (false);
	}
	else if (pthread_mutex_init(&table->full_mtx, NULL))
	{
		free_mtx(table->seats, table, 'a');
		pthread_mutex_destroy(&table->ended_mtx);
		pthread_mutex_destroy(&table->write_mtx);
		free_data(table);
		return (false);
	}
	return (true);
}

static bool	init_philos(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < table->seats)
	{
		philo = table->phil + i;
		philo->table = table;
		philo->index = i + 1;
		philo->meals_eaten = 0;
		philo->meals_limit = table->meals_limit;
		philo->full = false;
		philo->last_meal_time = 0;
		if (pthread_mutex_init(&philo->philo_mtx, NULL))
		{
			free_mtx(i, table, 'p');
			free_mtx(table->seats, table, 'f');
			free_data(table);
			return (false);
		}
		assign_forks(philo, table->forks, i);
	}
	return (true);
}

static bool	init_forks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->seats)
	{
		if (pthread_mutex_init(&table->forks[i].mtx, NULL))
		{
			free_mtx(i, table, 'f');
			free_data(table);
			return (false);
		}
		table->forks[i].index = i;
	}
	return (true);
}

bool	init_data(t_table *table)
{
	table->ended = false;
	table->phil = malloc(sizeof(t_philo) * table->seats);
	if (!table->phil)
		return (false);
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free_data(table);
		return (false);
	}
	if (!init_forks(table))
		return (false);
	if (!init_philos(table))
		return (false);
	if (!init_mtx(table))
		return (false);
	return (true);
}
