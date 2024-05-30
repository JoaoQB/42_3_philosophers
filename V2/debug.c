/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:01:21 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/30 15:50:15 by jqueijo-         ###   ########.fr       */
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
int		is_digit(char c);
int		ft_atoi(const char *nptr);

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
void	ft_sleep(long usecs, t_table *table);

/* routine .c */
void	*routine(void *data);
void	*one_philo(void *data);

/* monitor.c */
void	*monitor_philos(void *data);

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
	free_mtx(table->seats, table, 'a');
	pthread_mutex_destroy(&table->ended_mtx);
	pthread_mutex_destroy(&table->write_mtx);
	pthread_mutex_destroy(&table->monitor_mtx);
	if (table->phil)
		free (table->phil);
	if (table->forks)
		free (table->forks);
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
	int		i;

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

/* init_dinner.c */
static bool	handle_one_philo(t_table *table)
{
	t_philo	*philo;

	philo = table->phil;
	if (pthread_create(&philo[0].thread_id, NULL, one_philo, &philo[0]))
	{
		printf("Error creating thread for  one philo.\n");
		return (false);
	}
	if (pthread_create(&table->monitor, NULL, monitor_philos, table) != 0)
	{
		printf("Error creating monitor thread.\n");
		return (false);
	}
	return (true);
}

static bool	join_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->phil;
	i = -1;
	if (pthread_join(table->monitor, NULL))
		return (false);
	while (++i < table->seats)
	{
		if (pthread_join(philo[i].thread_id, NULL))
			return (false);
	}
	return (true);
}

static bool	create_threads(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->phil;
	i = -1;
	while (++i < table->seats)
	{
		if (pthread_create(&philo[i].thread_id, NULL, routine, &philo[i]))
			return (false);
	}
	if (pthread_create(&table->monitor, NULL, monitor_philos, table))
		return (false);
	return (true);
}

bool	init_dinner(t_table *table)
{
	table->start_time = get_time();
	if (table->start_time == -1)
		return (false);
	if (table->meals_limit == 0)
		return (false);
	else if (table->seats == 1)
	{
		if(!handle_one_philo(table))
			return (false);
	}
	else
	{
		if (!create_threads(table))
			return (false);
	}
	if (!join_threads(table))
		return (false);
	return (true);
}

/* monitor.c */
static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&philo->philo_mtx, &philo->full))
		return (false);
	pthread_mutex_lock(&philo->philo_mtx);
	elapsed = get_time() - philo->last_meal_time;
	time_to_die = philo->table->time_to_die;
	pthread_mutex_unlock(&philo->philo_mtx);
	if (elapsed >= time_to_die)
		return (true);
	return (false);
}

static bool	check_philos(t_table *table, int *served)
{
	int	i;

	i = -1;
	while (++i < table->seats)
	{
		if (philo_died(table->phil + i))
		{
			print_status(table->phil + i, RD"has died"RST);
			set_bool(&table->ended_mtx, &table->ended, true);
			return (true);
		}
		else if (get_bool(&table->phil[i].philo_mtx, &table->phil[i].full))
		{
			(*served)++;
			if (*served == table->seats)
			{
				set_bool(&table->ended_mtx, &table->ended, true);
				return (true);
			}
		}
	}
	return (false);
}

void	*monitor_philos(void *data)
{
	t_table	*table;
	int		served;

	served = 0;
	table = (t_table *)data;
	while (1)
	{
		if (get_bool(&table->ended_mtx, &table->ended))
			return (NULL);
		if (check_philos(table, &served))
			return (NULL);
	}
	return (NULL);
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
		|| table->time_to_sleep < 60 || table->seats < 1)
		return (false);
	if (argv[5])
		table->meals_limit = ft_atoi(argv[5]);
	else
		table->meals_limit = -1;
	return (true);
}

/*routine.c */
void	*one_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->philo_mtx);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	print_status(philo, "has taken the first fork");
	while (1)
	{
		if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
			break ;
		ft_sleep(5, philo->table);
	}
	return (NULL);
}
static void	think(t_philo *philo)
{
	long	last_ate_elapsed;
	long	time_to_think;

	last_ate_elapsed = get_time() - philo->last_meal_time;
	time_to_think = last_ate_elapsed / 2;
	if (time_to_think + last_ate_elapsed > philo->table->time_to_die)
		time_to_think = 1;
	print_status(philo, "is thinking");
	ft_sleep(time_to_think, philo->table);
}

static void	rest(t_philo *philo)
{
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	print_status(philo, "is sleeping");
	ft_sleep(philo->table->time_to_sleep, philo->table);
}

static void	eat(t_philo *philo)
{
	if (philo->meals_eaten > 0 && philo->meals_eaten == philo->meals_limit)
	{
		set_bool(&philo->philo_mtx, &philo->full, true);
		return ;
	}
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	pthread_mutex_lock(&philo->left_fork->mtx);
	print_status(philo, "has taken the first fork");
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
	{
		pthread_mutex_unlock(&philo->left_fork->mtx);
		return ;
	}
	pthread_mutex_lock(&philo->right_fork->mtx);
	print_status(philo, "has taken the second fork");
	pthread_mutex_lock(&philo->philo_mtx);
	philo->meals_eaten++;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	print_status(philo, GRN"is eating"RST);
	ft_sleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_unlock(&philo->right_fork->mtx);
	pthread_mutex_unlock(&philo->left_fork->mtx);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->philo_mtx);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->philo_mtx);
	while (1)
	{
		if (get_bool(&philo->philo_mtx, &philo->full))
			break ;
		if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
			break ;
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
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

/* utils.c */
void	ft_sleep(long usecs, t_table *table)
{
	long	start;

	start = get_time();
	if (usecs < 10)
	{
		usleep(usecs);
		return ;
	}
	while (get_time() - start < usecs)
	{
		if (get_bool(&table->ended_mtx, &table->ended))
			break ;
		usleep(usecs / 10);
	}
	return ;
}

void	print_status(t_philo *philo, char *status)
{
	long	elapsed;

	elapsed = get_time() - philo->table->start_time;
	if (get_bool(&philo->table->ended_mtx, &philo->table->ended))
		return ;
	if (pthread_mutex_lock(&philo->table->write_mtx))
		write(2, "print failed", 13);
	printf(WHT"%ld"RST BL" %d"RST" %s\n", elapsed, philo->index, status);
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

/* main.c */
static void	print_args(t_table *table)
{
	int		i;

	printf("Seats: %d\n", table->seats);
	printf("Time to die: %d\n", table->time_to_die);
	printf("Time to eat: %d\n", table->time_to_eat);
	printf("Time to sleep: %d\n", table->time_to_sleep);
	printf("Meals limit: %d\n", table->meals_limit);
	for (i = 0; i < table->seats; i++)
	{
		printf("Philosopher %d was assigned forks %d and %d\n",
			table->phil[i].index,
			table->phil[i].left_fork->index,
			table->phil[i].right_fork->index);
	}
}

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
		printf(RD"Values must be > 60 and < INT_MAX\n"RST);
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
			return (2);
		// print_args(&table);
		if (!init_dinner(&table))
			return (3);
		cleanup(&table);
	}
	return (0);
}
