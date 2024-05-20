/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_onepagecode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 13:07:47 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/18 14:46:50 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* One page only, philo.h */
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
	t_table		*table;
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
int		parse_input(char **argv, t_table *table);

/* string_utils.c */
int		is_digit(char c);
int		ft_atoi(const char *nptr);

/* init.c */
int		start_dinner(t_table *table);

/* One page only, init.c */
static void	assign_forks(t_philo *philo, t_fork *forks, int position)
{
	int	philo_nbr;

	philo_nbr = philo->seats;
	if (philo->index % 2 == 0)
	{
		philo->righ_fork = &forks[position];
		philo->left_fork = &forks[(position + 1) % philo_nbr];
	}
	else
	{
		philo->righ_fork = &forks[(position + 1) % philo_nbr];
		philo->left_fork = &forks[position];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while(++i < table->seats)
	{
		philo = table->philosophers + i;
		philo->index = i + 1;
		philo->meals_eaten = 0;
		philo->is_full = false;
		philo->dead = false;
		philo->seats = table->seats;
		philo->time_to_die = table->time_to_die;
		philo->time_to_eat = table->time_to_eat;
		philo->time_to_sleep = table->time_to_sleep;
		philo->meals_limit = table->meals_limit;
		assign_forks(philo, table->forks, i);

		printf("Philosopher %d is at position %d\n", philo->index, i);
		printf("Philosopher %d has right fork %d and left fork %d\n",
				philo->index,
				philo->righ_fork->index,
				philo->left_fork->index);
	}
}

static int	init_forks(t_table *table)
{
	int	i;
	int	j;

	i = -1;
	while(++i < table->seats)
	{
		if(pthread_mutex_init(&table->forks[i].mtx, NULL) != 0)
		{
			j = -1;
			while (++j < i)
				pthread_mutex_destroy(&table->forks[j].mtx);
			return (0);
		}
		table->forks[i].index = i;
	}
	return (1);
}

int	start_dinner(t_table *table)
{
	table->ended = false;
	table->philosophers = malloc(sizeof(t_philo) * table->seats);
	if (!table->philosophers)
		return 1;
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free(table->philosophers);
		return 1;
	}
	if (!init_forks(table))
	{
		free(table->philosophers);
		free(table->forks);
		return 1;
	}
	philo_init(table);
	return (0);
}

/* One page only, parsing.c */
static int	validate_input(char *argv)
{
	int	i;

	if (!argv)
		return (1);
	i = 0;
	while (argv[i])
	{
		if (!is_digit(argv[i]))
			return (0);
		if (i > 10)
			return (0);
		i++;
	}
	return (1);
}

int	parse_input(char **argv, t_table *table)
{
	if (!validate_input(argv[1]) || !validate_input(argv[2])
		|| !validate_input(argv[3]) || !validate_input(argv[4])
		|| !validate_input(argv[5]))
		return (0);
	table->seats = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		return (0);
	if (argv[5])
		table->meals_limit = ft_atoi(argv[5]);
	else
		table->meals_limit = -1;
	printf("seats: %d\n", table->seats);
	printf("time_to_die: %d\n", table->time_to_die);
	printf("time_to_eat: %d\n", table->time_to_eat);
	printf("time_to_sleep: %d\n", table->time_to_sleep);
	printf("meals_limit: %d\n", table->meals_limit);
	return (1);
}

/* string_utils.c */
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

int	main(int argc, char **argv)
{
	t_table	table;
	// DON'T FORGET TO CHANGE MAKEFILE FLAGS!!!! DUMMY
	if (argc == 5 || argc == 6)
	{
		if (!parse_input(argv, &table))
		{
			printf(RED"Please only use digits;\n"RESET);
			printf(RED"Values bigger than 60;\nSmaller than INT_MAX.\n"RESET);
			return (1);
		}
		start_dinner(&table);
	}
	else
	{
		printf("Wrong input:\n"
			GREEN"Please input i.e. ./philo 6 400 200 200 [10]\n"RESET);
		return (1);
	}
}
