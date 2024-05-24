/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:01:21 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/24 11:19:42 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* philo.h */
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
	t_philo	*phil;
	t_fork	*forks;
	t_mtx	ended_mtx;
	t_mtx	write_mtx;
	t_mtx	monitor_mtx;
	int		seats;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		meals_limit;
	int		served;
	int		start_time;
	bool	ended;
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

#endif

/* cleanup.c */
void	free_mtx(int i, t_table *table, char c)
{
	int	j;

	j = -1;
	if (c == 'f')
	{
		while (++j < i)
			pthread_mutex_destroy(&table->forks[j].mtx);
	}
	else if (c == 'p')
	{
		while (++j < i)
			pthread_mutex_destroy(&table->phil[j].philo_mtx);
	}
	else if (c == 'a')
	{
		while (++j < i)
			pthread_mutex_destroy(&table->forks[j].mtx);
		j = -1;
		while (++j < i)
			pthread_mutex_destroy(&table->phil[j].philo_mtx);
	}
	return ;
}

void	free_data(t_table *table)
{
	if (table->phil)
		free (table->phil);
	if (table->forks)
		free (table->forks);

}

void	cleanup(t_table *table)
{
	printf("clean");
}

/* init_data.c */
static void	assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->table->seats;
	if (philo->index % 2 == 0)
	{
		philo->left_fork = &forks[position];
		philo->right_fork = &forks[(position + 1) % philo_nbr];
	}
	else
	{
		philo->left_fork = &forks[(position + 1) % philo_nbr];
		philo->right_fork = &forks[position];
	}
}

static bool	init_mtx(t_table *table)
{
	if (pthread_mutex_init(&table->ended_mtx, NULL))
	{
		return (false);
	}
	else if (pthread_mutex_init(&table->write_mtx, NULL))
	{
			free_mtx(table->seats, table, 'a');
			pthread_mutex_destroy(&table->ended_mtx);
			free_data(table);
			return (false);
	}
	else if (pthread_mutex_init(&table->monitor_mtx, NULL))
	{
			free_mtx(table->seats, table, 'a');
			pthread_mutex_destroy(&table->ended_mtx);
			pthread_mutex_destroy(&table->write_mtx);
			free_data(table);
			return (false);
	}
	return (true);
}

static bool	init_philos(t_table *table)
{
	t_philo	*philo;
	int	i;

	i = -1;
	while (++i < table->seats)
	{
		philo = table->phil + i;
		philo->table = table;
		philo->index = i + 1;
		philo->meals_eaten = 0;
		philo->meals_limit = table->meals_limit;
		philo->full = false;
		if (pthread_mutex_init(&philo->philo_mtx, NULL))
		{
			free_mtx(i, table, 'p');
			free_mtx(table->seats, table, 'f');
			free_data(table);
			return (false);
		}
		assign_forks(philo, table->forks, i);
	}
	return (true);
}

static bool	init_forks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->seats)
	{
		if (pthread_mutex_init(&table->forks[i].mtx, NULL))
		{
			free_mtx(i, table, 'f');
			free_data(table);
			return (false);
		}
		table->forks[i].index = i;
	}
	return (true);
}

bool	init_data(t_table *table)
{
	table->ended = false;
	table->phil = malloc(sizeof(t_philo) * table->seats);
	if (!table->phil)
		return (false);
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free_data(table);
		return (false);
	}
	if (!init_forks(table))
		return (false);
	if (!init_philos(table))
		return (false);
	if (!init_mtx(table))
		return (false);
	return (true);
}

/* parsing .c */
static bool	validate_input(char *argv)
{
	int	i;

	if (!argv)
		return (true);
	i = -1;
	while (argv[++i])
	{
		if (!is_digit(argv[i]))
			return (false);
		if (i > 10)
			return (false);
	}
	return (true);
}

bool	parse_input(char **argv, t_table *table)
{
	if (!validate_input(argv[1]) || !validate_input(argv[2])
		|| !validate_input(argv[3]) || !validate_input(argv[4])
		|| !validate_input(argv[5]))
		return (false);
	table->seats = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		return (false);
	if (argv[5])
		table->meals_limit = ft_atoi(argv[5]);
	else
		table->meals_limit = -1;
	return (true);
}

/* str_utils.c */
int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *nptr)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	i = 0;
	sign = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (is_digit(nptr[i]))
	{
		res = (res * 10) + nptr[i] - '0';
		i++;
	}
	if (res > INT_MAX)
		return (0);
	return (res * sign);
}

/* main.c */
static void	print_error(char c)
{
	if (c == 'a')
	{
		printf("Wrong input:\n"
			GRN"Please input i.e. ./philo 6 400 200 200 [10]\n"RST);
	}
	else if (c == 'p')
	{
		printf(RD"Please only use digits;\n"RST);
		printf(RD"Values must be > 60 and < INT_MAX\n");
	}
	return ;
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
	{
		print_error('a');
		return (1);
	}
	else
	{
		if (!parse_input(argv, &table))
		{
			print_error('p');
			return (1);
		}
		if (!init_data(&table))
			return (1);
	}
}
