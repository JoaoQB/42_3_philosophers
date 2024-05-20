/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gettimeofday.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:20:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/20 09:28:06 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../philo.h"

int main() {
	struct timeval start, end;
	long elapsed, elapsed_miliseconds;

	// Get the start time
	gettimeofday(&start, NULL);

	// Perform some operations
	for (long i = 0; i < 1000000; ++i);

	// Get the end time
	gettimeofday(&end, NULL);

	// Calculate the elapsed time in microseconds
	elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	elapsed_miliseconds = elapsed / 1000;

	printf("Elapsed time: %ld microseconds\n", elapsed);
	printf("Elapsed time: %ld miliseconds\n", elapsed_miliseconds);

	return 0;
}
