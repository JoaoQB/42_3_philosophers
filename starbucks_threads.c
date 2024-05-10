/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starbucks_threads.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:47:48 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/10 13:12:50 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void	*make_coffee(void *data)
{
	printf("Barista is making coffee...\n");
	sleep(2);
	printf("coffee ready\n");
	return (NULL);
}

int	main()
{
	pthread_t	barista_1;
	pthread_t	barista_2;

	pthread_create(&barista_1, NULL, make_coffee, NULL);
	pthread_create(&barista_2, NULL, make_coffee, NULL);
}
