/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:39:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 11:57:58 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time() - philo->table->start_time;
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	if (pthread_mutex_lock(&philo->table->write_mtx))
		write(2, "print failed", 13);
	printf(WHT"%ld"RST BL" %d"RST" %s\n", elapsed, philo->index, status);
	pthread_mutex_unlock(&philo->table->write_mtx);
}

void	set_bool(t_mtx *mtx, bool *dest, bool value)
{
	pthread_mutex_lock(mtx);
	*dest = value;
	pthread_mutex_unlock(mtx);
}

bool	get_bool(t_mtx *mtx, bool *value)
{
	bool	ret;

	pthread_mutex_lock(mtx);
	ret = *value;
	pthread_mutex_unlock(mtx);
	return (ret);
}

long	get_time(void)
{
	struct timeval	tv;
	long			time_of_day;

	if (gettimeofday(&tv, NULL))
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	time_of_day = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_of_day);
}
