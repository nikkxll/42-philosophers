/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 16:41:50 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

/* all using constants, enums and external headers are lying here */
# include "constants_bonus.h"

typedef struct timeval	t_time;

/* structure to keep all arguments passed */
typedef struct s_input
{
	long	num_of_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	eat_number;
}	t_input;

/* structure to keep simulation related variables */
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

/* cleaner: to free all the memory that's been allocated, destroy semaphores
and print corresponding message if there is an error */
void		kill_process(t_shared *shared, int i, int j);
void		semaphore_closing(t_shared *shared);
int			semaphore_clearing(t_shared *shared, int flag, int i, int j);
int			error_msg(char *msg);
int			struct_free(t_shared *shared, char *message, int status);

/* simulation: simulation process itself */
int			philosophers(t_shared *shared);

/* libft: functions from libft in separate func since libft is
not authorized */
long		ft_atol(const char *str);
size_t		ft_strlen(const char *s);
void		*ft_calloc(size_t count, size_t size);

/* printing: print messages before think, eat or sleep */
void		print_message(int procedure, t_shared *shared);

/* processing: initialization before simulation */
int			initialization(int ac, char **av, t_shared *shared);

/* reader: part of convertion from arguments vector + error handlers */
int			reader(int ac, char **av, t_shared *shared);

/* utils: a couple of auxiliary functions for the routine */
long long	get_timestamp(t_shared *shared);
void		ft_usleep(int ms, t_shared *shared);
int			semaphore_wait_control(t_shared *shared);
int			semaphore_post_control(t_shared *shared);
void		process_error(t_shared *shared, char *msg);

/* wrappers: semaphores and thread wrappers to handle possible errors */
int			thread_wrapper(pthread_t *thread, void *(*foo)(void *),
				void *data, t_code code);
int			sem_wrapper(sem_t *semaphore, t_code code);
void		wait_post_wrapper(t_shared *shared, sem_t *semaphore,
				int procedure);
#endif