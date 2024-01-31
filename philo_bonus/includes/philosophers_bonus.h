/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 00:35:00 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

/* all using constants, enums and external headers are lying here */
# include "constants_bonus.h"

typedef struct timeval	t_time;

/* structure to keep all arguments passed + solo as a flag if
there is only one philosopher */
typedef struct s_input
{
	long	num_of_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	eat_number;
}	t_input;

/* structure to keep shared variables between threads */
typedef struct s_shared
{
	pid_t		*pid;
	int			meals;
	long long	last_meal_ts;
	t_time		tv;
	pthread_t	monitor;
	t_input		*input;
	int			philo_id;
	sem_t		*forks;
	sem_t		*locker;
	sem_t		*end;
	sem_t		*death;
}	t_shared;

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
long long	get_timestamp(t_shared *shared);
void		ft_usleep(int ms, t_shared *shared);
void		print_message(int procedure, t_shared *shared);
int			semaphore_clearing(t_shared *shared);
void		semaphore_parent_control(t_shared *shared);
int			sem_wrapper(sem_t *semaphore, t_code code);
void		semaphore_closing(t_shared *shared);
void		wait_post_wrapper(t_shared *shared, sem_t *semaphore, int procedure);

/* procedures: eating and sleeping procedures */

/* reader: part of convertion from arguments vector + error handlers */
int			reader(int ac, char **av, t_shared *shared);

/* threads_utils: a couple of auxiliary functions for the routine */
void		flag_locker(t_shared *shared, int status);
int			meals_checker(int *arr, int size, int eats);

/* wrappers: mutex and thread wrappers to handle possible errors */
int			mutex_wrapper(pthread_mutex_t *mutex, t_code code);
int			thread_wrapper(pthread_t *thread, void *(*foo)(void *),
				void *data, t_code code);

#endif