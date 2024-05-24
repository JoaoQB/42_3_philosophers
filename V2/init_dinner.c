/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dinner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:36:57 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 11:51:59 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	join_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->phil;
	i = -1;
	if (pthread_join(table->monitor, NULL))
		return (false);
	while (++i < table->seats)
	{
		if (pthread_join(philo[i].thread_id, NULL))
			return (false);
	}
	return (true);
}

static bool	create_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->phil;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_create(&philo[i].thread_id, NULL, routine, &philo[i]))
			return (false);
	}
	if (pthread_create(&table->monitor, NULL, monitor_philos, table))
		return (false);
	return (true);
}

bool	init_dinner(t_table *table)
{
	table->start_time = get_time();
	if (table->start_time == -1)
		return (false);
	if (table->meals_limit == 0)
		return (false);
	// else if (table->seats == 1)
	// {

	// }
	else
	{
		if (!create_threads(table))
			return (false);
	}
	if (join_threads(table))
		return (false);
	return (true);
}
