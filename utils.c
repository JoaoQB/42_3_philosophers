/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:42 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/20 11:37:47 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(long usecs)
{
	long	start;
	start = get_time();
	while (get_time() - start < usecs)
		usleep(usecs / 10);
	return ;
}

void	print_status(t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time() - philo->start_time;
	if (get_bool(&philo->table_mtx, &philo->ended))
		return ;
	pthread_mutex_lock(&philo->table_mtx);
	printf(WHITE"%ld"RESET  BLUE"%d"RESET" %s\n", elapsed, philo->index, status);
	pthread_mutex_unlock(&philo->table_mtx);
}

bool	get_bool(pthread_mutex_t *mtx, bool *value)
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
