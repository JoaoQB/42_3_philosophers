/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 11:36:01 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 12:02:13 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h> // write, usleep
# include <stdio.h> // printf
# include <stdlib.h> // malloc, free
# include <stdbool.h> // bools
# include <pthread.h> // mutex, threads
# include <sys/time.h> // gettimeofday
# include <limits.h> // INT_MAX

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
	pthread_t	thread_id;
	t_table		*table;
	t_fork		*left_fork;
	t_fork		*right_fork;
	t_mtx		philo_mtx;
	int			index;
	long		last_meal_time;
	int			meals_eaten;
	int			meals_limit;
	bool		full;
}	t_philo;

struct s_table
{
	t_philo		*phil;
	t_fork		*forks;
	pthread_t	monitor;
	t_mtx		ended_mtx;
	t_mtx		write_mtx;
	t_mtx		monitor_mtx;
	int			seats;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			meals_limit;
	int			start_time;
	bool		ended;
};

/* parsing.c */
bool	parse_input(char **argv, t_table *table);

/* str_utils.c */
int	is_digit(char c);
int	ft_atoi(const char *nptr);

/* init_data.c */
bool	init_data(t_table *table);

/* cleanup.c */
void	free_data(t_table *table);
void	free_mtx(int i, t_table *table, char c);
void	cleanup(t_table *table);

/* init_dinner.c */
bool	init_dinner(t_table *table);

/* utils.c */
long	get_time(void);
bool	get_bool(t_mtx *mtx, bool *value);
void	set_bool(t_mtx *mtx, bool *dest, bool value);
void	print_status(t_philo *philo, char *status);

/* routine .c */
void	*routine(void *data);

/* monitor.c */
void	*monitor_philos(void *data);

#endif
