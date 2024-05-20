/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_onepagecode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 13:07:47 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/20 19:27:57 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* One page only, philo.h */
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

typedef struct s_table	t_table;

typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	mtx;
	int				index;
}	t_fork;

typedef struct s_philo
{
	pthread_t		thread_id;
	t_table			*table;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_mtx			philo_mtx;
	// t_mtx			write_mtx;
	int				index;
	int				meals_eaten;
	long			last_meal_time;
	bool			is_full;
	bool			dead;
	int				meals_limit;
	// int				seats;
	// int				time_to_die;
	// int				time_to_eat;
	// int				time_to_sleep;
	// long			start_time;
	// bool			ended;
}	t_philo;

struct s_table
{
	t_philo			*philos;
	t_fork			*forks;
	t_mtx			mtx;
	t_mtx			write_mtx;
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

/* utils.c */
long	get_time(void);
bool	get_bool(t_mtx *mtx, bool *value);
void	set_bool(t_mtx *mtx, bool *dest, bool value);
void	print_status(t_philo *philo, char *status);
void	ft_sleep(long usecs, t_table *table);

/* cleanup.c */

#endif

/* One page only, dining.c */
static int	create_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_create(&philo[i].thread_id, NULL, routine, &philo[i]))
		{
			printf("Error creating thread for philo %d.\n", philo[i].index);
			return (-1);
		}
	}
	return (1);
}

static int	join_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_join(philo[i].thread_id, NULL))
		{
			printf("Error joining thread for philo %d.\n", philo[i].index);
			return (-1);
		}
	}
	return (1);
}

int	run_dinner(t_table *table)
{
	if (table->meals_limit == 0)
		return (1);
	// else if (table->seats == 1)
	// 	// TODO
	if (create_threads(table) == -1)
		return (1);
	table->start_time = get_time();
	if (table->start_time == -1)
		return (1);
	printf("Dinner started at: %ld miliseconds.\n", table->start_time);
	if (join_threads(table) == -1)
		return (1);
	return (0);
}

/* One page only, init.c */
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

static void	philo_init(t_table *table)
{
	int		i;
	int		j;
	t_philo	*philo;

	i = -1;
	while (++i < table->seats)
	{
		philo = table->philos + i;
		philo->index = i + 1;
		philo->meals_eaten = 0;
		philo->is_full = false;
		philo->dead = false;
		philo->meals_limit = table->meals_limit;
		// philo->seats = table->seats;
		// philo->time_to_die = table->time_to_die;
		// philo->time_to_eat = table->time_to_eat;
		// philo->time_to_sleep = table->time_to_sleep;
		// philo->table_mtx = table->mtx;
		// philo->ended = table->ended;
		assign_forks(philo, table->forks, i);
		if (pthread_mutex_init(&philo->philo_mtx, NULL))
		{
			j = -1;
			while (++j < i)
				pthread_mutex_destroy(&philo->philo_mtx);
		}
		printf("Philosopher %d is at position %d\n", philo->index, i);
		printf("Philosopher %d has right fork %d and left fork %d\n",
			philo->index,
			philo->right_fork->index,
			philo->left_fork->index);
	}
}

static int	init_forks(t_table *table)
{
	int	i;
	int	j;

	i = -1;
	while (++i < table->seats)
	{
		if (pthread_mutex_init(&table->forks[i].mtx, NULL) != 0)
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

int	init_dinner(t_table *table)
{
	table->ended = false;
	table->philos = malloc(sizeof(t_philo) * table->seats);
	if (!table->philos)
		return (1);
	table->forks = malloc(sizeof(t_fork) * table->seats);
	if (!table->forks)
	{
		free(table->philos);
		return (1);
	}
	if (!init_forks(table))
	{
		free(table->philos);
		free(table->forks);
		return (1);
	}
	philo_init(table);
	if (pthread_mutex_init(&table->mtx, NULL)
		|| pthread_mutex_init(&table->write_mtx, NULL))
	{
		pthread_mutex_destroy(&table->mtx);
		pthread_mutex_destroy(&table->write_mtx);
		return (1);
	}
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

/* routine.c */
static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mtx);
	print_status(philo, "has taken the first fork");
	pthread_mutex_lock(&philo->right_fork->mtx);
	print_status(philo, "has taken the second fork");
	pthread_mutex_lock(&philo->philo_mtx);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	philo->meals_eaten++;
	print_status(philo, "is eating");
	ft_sleep(philo->table->time_to_eat, philo->table);
	if (philo->meals_limit > 0
		&& philo->meals_eaten == philo->meals_limit)
		set_bool(&philo->philo_mtx, &philo->is_full, true);
	pthread_mutex_unlock(&philo->left_fork->mtx);
	pthread_mutex_unlock(&philo->right_fork->mtx);
}

static void	rest(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_sleep(philo->table->time_to_sleep, philo->table);
}

static void	think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (!get_bool(&philo->table->mtx, &philo->table->ended))
	{
		if (get_bool(&philo->philo_mtx, &philo->is_full))
			break ;
		if (get_bool(&philo->philo_mtx, &philo->table->ended))
			break ;
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
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

/* utils.c */
void	ft_sleep(long usecs, t_table *table)
{
	long	start;
	start = get_time();
	while (get_time() - start < usecs)
	{
		if (get_bool(&table->mtx, &table->ended))
			break ;
		usleep(usecs / 10);
	}
	return ;
}

void	print_status(t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time() - philo->table->start_time;
	if (get_bool(&philo->table->mtx, &philo->table->ended))
		return ;
	pthread_mutex_lock(&philo->table->write_mtx);
	printf(WHITE"%ld"RESET  BLUE"%d"RESET" %s\n", elapsed, philo->index, status);
	pthread_mutex_unlock(&philo->table->write_mtx);
}

void	set_bool(t_mtx *mtx, bool *dest, bool value)
{
	pthread_mutex_lock(mtx);
	*dest = value;
	pthread_mutex_unlock(mtx);
}

bool	get_bool(t_mtx *mtx, bool *value)
{
	bool	ret;

	pthread_mutex_lock(mtx);
	ret = *value;
	pthread_mutex_unlock(mtx);
	return (ret);
}

long	get_time(void)
{
	struct timeval	tv;
	long			time_of_day;

	if (gettimeofday(&tv, NULL))
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	time_of_day = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_of_day);
}
