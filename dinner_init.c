/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:29:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/21 17:12:09 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	table_mtx_init(t_table *table)
{
	if (pthread_mutex_init(&table->mtx, NULL) != 0)
	{
		printf("table mutex failed");
		pthread_mutex_destroy(&table->mtx);
		return (1);
	}
	if (pthread_mutex_init(&table->monitor_mtx, NULL) != 0)
	{
		printf("table_write mutex failed");
		pthread_mutex_destroy(&table->monitor_mtx);
		return (1);
	}
	printf("table mutexes successfully initialized\n");
	return (0);
}

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
	printf("philo %d forks successfully assigned\n", position + 1);
}

static int	philo_init(t_table *table)
{
	int		i;
	int		j;
	t_philo	*philo;

	i = -1;
	while (++i < table->seats)
	{
		philo = table->philos + i;
		philo->table = table;
		philo->index = i + 1;
		philo->meals_eaten = 0;
		philo->is_full = false;
		philo->dead = false;
		philo->meals_limit = table->meals_limit;
		if (pthread_mutex_init(&philo->philo_mtx, NULL) != 0)
		{
			printf("philo_mtx failed\n");
			j = -1;
			while (++j < i)
				pthread_mutex_destroy(&philo->philo_mtx);
			return (-1);
		}
		assign_forks(philo, table->forks, i);
		printf("philo %d successfully initialized\n", i + 1);
	}
		return (0);
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
			printf("forks mutex failed\n");
			j = -1;
			while (++j < i)
				pthread_mutex_destroy(&table->forks[j].mtx);
			return (0);
		}
		table->forks[i].index = i;
	}
	printf("forks successfully initialized\n");
	return (1);
}

int	init_dinner(t_table *table)
{
	table->ended = false;
	table->philos = malloc(sizeof(t_philo) * table->seats);
	if (!table->philos)
		return (1);
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free(table->philos);
		return (1);
	}
	if (!init_forks(table))
	{
		free(table->philos);
		free(table->forks);
		return (1);
	}
	if (table_mtx_init(table))
		return (1);
	if (philo_init(table))
		return (1);
	return (0);
}
