/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/29 17:40:38 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* all using constants, enums and external headers are lying here */
# include "constants.h"

typedef struct s_philo	t_philo;
typedef struct timeval	t_time;
typedef pthread_mutex_t	t_mutex;

/* structure to keep all arguments passed + solo as a flag if
there is only one philosopher */
typedef struct s_input
{
	long	num_of_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	eat_number;
	int		solo;
}	t_input;

/* structure to keep shared variables between threads */
typedef struct s_shared
{
	int			flag_locker;
	int			exit_flag;
	int			*meals;
	long long	*last_meal_ts;
	t_mutex		locker;
	t_mutex		meal;
	t_mutex		*fork_mutex;
	t_time		tv;
	pthread_t	monitor;
	t_philo		**philo;
	t_input		*input;
}	t_shared;

/* template structure for every philosopher with links on shared
and input structures */
typedef struct s_philo
{
	t_shared	*shared;
	t_input		*input;
	pthread_t	philo_pth;
	int			philo_id;
}	t_philo;

/* chechers: if somebody have already died or everyone finished
their meal streak */
int			meals_finish_check(t_shared *shared);
int			death_check(t_shared *shared, int j);

/* cleaner: to free all the memory that's been allocated, destroy mutexes
and print corresponding message if there is an error */
int			error_msg(char *msg);
int			struct_free(t_shared *shared, char *message, int status);
int			mutex_cleaner(t_shared *shared, int current, int i);

/* initialization: init variables from shared structure */
int			initialization(int ac, char **av, t_shared *shared);

/* libft: functions from libft in separate func since libft is
not authorized */
long		ft_atol(const char *str);
size_t		ft_strlen(const char *s);
void		*ft_calloc(size_t count, size_t size);

/* philosophers: core part of the program with philo and monitoring
routines */
int			philosophers(t_shared *shared);

/* printing: print messages before think, eat or sleep */
void		print_message(int procedure, t_philo *philo);

/* procedures: eating and sleeping procedures */
void		ft_sleep_delay(int procedure, t_philo *philo);
void		ft_eating(t_philo *philo, int left, int right);

/* reader: part of convertion from arguments vector + error handlers */
int			reader(int ac, char **av, t_shared *shared);

/* threads_utils: a couple of auxiliary functions for the routine */
void		flag_locker(t_shared *shared, int status);
int			meals_checker(int *arr, int size, int eats);
void		meal_timestamp(t_philo *philo);
long long	get_timestamp(t_philo *philo);
void		ft_usleep(int ms, t_philo *philo);

/* wrappers: mutex and thread wrappers to handle possible errors */
int			mutex_wrapper(pthread_mutex_t *mutex, t_code code);
int			thread_wrapper(pthread_t *thread, void *(*foo)(void *),
				void *data, t_code code);

#endif