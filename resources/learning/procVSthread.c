/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procVSthread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:53:18 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/13 18:00:32 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wait.h>

uint64_t	ledger;

int	main(void)
{
	pid_t	pid;

	pid = fork();

	// CHILD
	if (pid == 0)
	{
		ledger += 42;
		printf("CHILD ledger-> %lu\n", ledger);
	}

	// I AM YOUR FATHER
	else
	{
		ledger += 42;
		wait(NULL);
		printf("FATHER ledger-> %lu\n", ledger);
	}
}
