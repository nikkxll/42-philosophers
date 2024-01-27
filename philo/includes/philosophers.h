/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 23:12:22 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "constants.h"
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;
typedef struct timeval	t_time;
typedef pthread_mutex_t	t_mutex;

typedef struct s_input
{
	long	num_of_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	eat_number;
	int		args_number;
}	t_input;

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

typedef struct s_philo
{
	t_shared	*shared;
	t_input		*input;
	pthread_t	philo_pth;
	int			philo_id;
}	t_philo;

long	ft_atol(const char *str);
int		reader(int ac, char **av, t_shared *shared);
int		error_msg(char *msg);
int		struct_free(t_shared *shared, char *message, int status);
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t count, size_t size);
int		initialization(int ac, char **av, t_shared *shared);
int		data_initialization(t_shared *shared, int i);
int		philosophers(t_shared *shared, int i, int error);

int		mutex_wrapper(pthread_mutex_t *mutex, t_code code);
int		thread_wrapper(pthread_t *thread, void *(*foo)(void *),
		void *data, t_code code);

#endif