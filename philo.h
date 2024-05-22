/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:08:45 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/22 14:37:30 by jqueijo-         ###   ########.fr       */
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
# define RST	"\033[0m"
# define RD		"\033[31m"
# define GRN	"\033[32m"
# define WHT   "\033[37m"
# define BL    "\033[34m"

typedef struct s_table	t_table;

typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	mtx;
	int		index;
}	t_fork;

typedef struct s_philo
{
	pthread_t		thread_id;
	t_table			*table;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_mtx			philo_mtx;
	int				index;
	int				meals_eaten;
	long			last_meal_time;
	bool			is_full;
	bool			dead;
	int				meals_limit;
}	t_philo;

struct s_table
{
	t_philo			*phil;
	t_fork			*forks;
	t_mtx			mtx;
	pthread_t		monitor;
	int				seats;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_limit;
	long			start_time;
	bool			ended;
};

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
void	*one_philo(void *data);

/* utils.c */
long	get_time(void);
bool	get_bool(t_mtx *mtx, bool *value);
void	set_bool(t_mtx *mtx, bool *dest, bool value);
void	print_status(t_philo *philo, char *status);
void	ft_sleep(long usecs, t_table *table);

/* monitor.c */
void	*monitor_philos(void *data);

/* cleanup.c */

#endif
