/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:08:45 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/20 11:37:09 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>   // printf
# include <stdlib.h>  // malloc, free
# include <unistd.h>  // write, usleep
# include <stdbool.h> // booleans
# include <pthread.h> // mutex: init destroy lock unlock
					// threads: create join detach
# include <sys/time.h>// gettimeofday
# include <limits.h>  // INT_MAX

// ANSI escape sequences
# define RESET	"\033[0m"
# define RED		"\033[31m"
# define GREEN	"\033[32m"
# define WHITE   "\033[37m"
# define BLUE    "\033[34m"


typedef struct s_fork
{
	pthread_mutex_t	mtx;
	int				index;
}	t_fork;

typedef struct s_philo
{
	pthread_t		thread_id;
	t_fork			*left_fork;
	t_fork			*righ_fork;
	pthread_mutex_t	table_mtx;
	int				index;
	int				meals_eaten;
	long			last_meal_time;
	bool			is_full;
	bool			dead;
	int				seats;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_limit;
	long			start_time;
	bool			ended;
}	t_philo;

typedef struct s_table
{
	t_philo			*philosophers;
	t_fork			*forks;
	pthread_mutex_t	mtx;
	int				seats;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_limit;
	long			start_time;
	bool			ended;
}	t_table;

/* parsing.c */
int		parse_input(char **argv, t_table *table);

/* string_utils.c */
int		is_digit(char c);
int		ft_atoi(const char *nptr);

/* init.c */
int		init_dinner(t_table *table);

/* dining.c */
int		run_dinner(t_table *table);

/* routine.c */
void	*routine(void *data);

/* utils.c */
long	get_time(void);
bool	get_bool(pthread_mutex_t *mtx, bool *value);
void	print_status(t_table *table, char *status);
void	ft_sleep(long usecs);

/* cleanup.c */

#endif
