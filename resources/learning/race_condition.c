/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   race_condition.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:55:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/13 17:29:55 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h> // thread programming
#include <stdlib.h>
#include <stdint.h> // standard integer

#define TH 100

// Shared resource
uint64_t	ledger = 0;

// Lock
pthread_mutex_t	ledger_lock = PTHREAD_MUTEX_INITIALIZER;

void	*update_ledger(void *data)
{
	pthread_mutex_lock(&ledger_lock);
	ledger += 1;
	pthread_mutex_unlock(&ledger_lock);
	return NULL;
}

int	main(void)
{
	pthread_t	waiters[TH];
	int			i;

	// pthread_mutex_init(&ledger_lock, NULL);
	i = -1;
	while (++i < TH)
	{
		if (pthread_create(waiters + i, NULL, update_ledger, NULL))
			exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < TH)
	{
		pthread_join(waiters[i], NULL);
	}
	printf("\t%lu\n", ledger);
	/* In terminal do:
	'while true; do
    ./a.out
done'
	*/
}
