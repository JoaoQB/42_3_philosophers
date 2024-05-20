/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dining.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:48:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/18 17:27:26 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	run_dinner(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = -1;
	if (table->meals_limit == 0)
		return ;
	// else if (table->seats == 1)
	// 	// TODO
	else
	{
		while (++i < table->seats)
		{
			philo = &table->philosophers[i];
			if(pthread_create(philo->thread_id, NULL, routine(), &philo));
				return ;
		}
	}
}
