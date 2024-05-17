/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:08:45 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/17 16:15:11 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, free
#include <unistd.h>  // write, usleep
#include <stdbool.h> // booleans
#include <pthread.h> // mutex: init destroy lock unlock
					// threads: create join detach
#include <sys/time.h>// gettimeofday
#include <limits.h>  // INT_MAX

// ANSI escape sequences
#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"

typedef struct s_fork
{
	pthread_mutex_t	mtx;
	int				index;
}	t_fork;

typedef struct s_philo
{
	pthread_t	thread_id;
	int			index;
	// *left_fork;
	// *right_fork;
	int			meals_eaten;
	long		last_meal_time;
	bool		is_full;
	bool		dead;
}	t_philo;

typedef struct s_table
{
	t_philo	*philosophers;
	t_fork	*forks;
	int		seats;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		meals_limit;
	bool	ended;
}	t_table;

/* parsing.c */
int	parse_input(char **argv, t_table *table);

/* string_utils.c */
int	is_digit(char c);
int	ft_atoi(const char *nptr);

/* init.c */
int	start_dinner(t_table *table);

/* cleanup.c */
