/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:29:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/20 11:20:42 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->seats;
	if (philo->index % 2 == 0)
	{
		philo->left_fork = &forks[position];
		philo->righ_fork = &forks[(position + 1) % philo_nbr];
	}
	else
	{
		philo->left_fork = &forks[(position + 1) % philo_nbr];
		philo->righ_fork = &forks[position];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->seats)
	{
		philo = table->philosophers + i;
		philo->index = i + 1;
		philo->meals_eaten = 0;
		philo->is_full = false;
		philo->dead = false;
		philo->seats = table->seats;
		philo->time_to_die = table->time_to_die;
		philo->time_to_eat = table->time_to_eat;
		philo->time_to_sleep = table->time_to_sleep;
		philo->meals_limit = table->meals_limit;
		philo->table_mtx = table->mtx;
		philo->ended = table->ended;
		assign_forks(philo, table->forks, i);
		printf("Philosopher %d is at position %d\n", philo->index, i);
		printf("Philosopher %d has right fork %d and left fork %d\n",
			philo->index,
			philo->righ_fork->index,
			philo->left_fork->index);
	}
}

static int	init_forks(t_table *table)
{
	int	i;
	int	j;

	i = -1;
	while (++i < table->seats)
	{
		if (pthread_mutex_init(&table->forks[i].mtx, NULL) != 0)
		{
			j = -1;
			while (++j < i)
				pthread_mutex_destroy(&table->forks[j].mtx);
			return (0);
		}
		table->forks[i].index = i;
	}
	return (1);
}

int	init_dinner(t_table *table)
{
	table->ended = false;
	table->philosophers = malloc(sizeof(t_philo) * table->seats);
	if (!table->philosophers)
		return (1);
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free(table->philosophers);
		return (1);
	}
	if (!init_forks(table))
	{
		free(table->philosophers);
		free(table->forks);
		return (1);
	}
	philo_init(table);
	return (0);
}
