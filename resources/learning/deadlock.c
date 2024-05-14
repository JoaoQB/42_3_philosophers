/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deadlock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:50:19 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/14 14:50:07 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_UNDERLINE     "\x1b[4m"

// GUNS
pthread_mutex_t ugly_gun = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bad_gun = PTHREAD_MUTEX_INITIALIZER;

typedef struct	s_cowboy
{
	char			*name;
	unsigned long	reaction_time;
	pthread_t		thread;
}	t_cowboy;

void	*action(void *data)
{
	t_cowboy	cowboy;

	cowboy = *(t_cowboy *)data;
	// How fast the cowboy is
	printf("%s reaction time "ANSI_BOLD ANSI_COLOR_CYAN"%lu\n"ANSI_COLOR_RESET,
		cowboy.name,
		cowboy.reaction_time);
	usleep(cowboy.reaction_time);

	if (!strcmp(cowboy.name, "ugly"))
		pthread_mutex_lock(&ugly_gun);
	else
		pthread_mutex_lock(&bad_gun);
	printf("%s has taken his own gun\n", cowboy.name);

	// I wanna take the other gun
	// DEADLOCK
	if (!strcmp(cowboy.name, "ugly"))
		pthread_mutex_lock(&bad_gun);
	else
		pthread_mutex_lock(&ugly_gun);

	// The killer will reach his position
	printf(ANSI_COLOR_RED"%s killed the other\n"ANSI_COLOR_RESET, cowboy.name);
	exit(EXIT_SUCCESS);
	return NULL;
}

int	main(void)
{
	srand((time(NULL)) * getpid());
	t_cowboy	ugly = {"ugly", rand() % 10000};
	t_cowboy	bad = {"bad", rand() % 10000};

	pthread_create(&ugly.thread, NULL, action, &ugly);
	pthread_create(&bad.thread, NULL, action, &bad);

	pthread_join(ugly.thread, NULL);
	pthread_join(bad.thread, NULL);
}
