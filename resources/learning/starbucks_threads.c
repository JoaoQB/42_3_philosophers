/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starbucks_threads.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:47:48 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/13 16:39:57 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void	*make_coffee(void *data)
{
	printf("Barista is making coffee...\n");
	sleep(2);
	printf("coffee ready\n");
	return (NULL);
}

int	main()
{
	pthread_t	baristas[500];
	int			i;

	i = -1;
	while (++i < 500)
	{
		if (pthread_create(baristas + i, NULL, make_coffee, NULL))
			exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < 500)
		printf("Thread id -> %lu\n", baristas[i]);
	i = -1;
	while (++i < 500)
		pthread_join(baristas[i], NULL);
}
