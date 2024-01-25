/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/25 18:59:31 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

# ifndef RED_COLOR
#  define RED_COLOR "\x1b[31m"
# endif

# ifndef RESET_COLOR
#  define RESET_COLOR "\x1b[0m"
# endif

typedef struct s_philo	t_philo;
typedef struct timeval	t_time;
typedef pthread_mutex_t	t_mutex;

typedef enum e_code
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_code;

typedef struct s_input
{
	long	num_of_philo;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	eat_number;
	int		args_number;
}	t_input;


typedef struct s_philo
{
	t_input		*input;
	pthread_t	*philosophers;
	pthread_t	monitor;
	t_mutex		*fork_mutex;
	t_mutex		locker;
	t_time		tv;
	int			*meals;
	long long	*last_meal_ts;
	int			*philo_ids;
	int			philo_id;
	int			flag_locker;
}	t_philo;


long	ft_atol(const char *str);
int		reader(int ac, char **av, t_philo *philo);
int		error_msg(char *msg);
int		error_struct_free(t_philo *philo, char *message);
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t count, size_t size);
int		initialization(int ac, char **av, t_philo *philo);
int		data_initialization(t_philo *philo);
int		philosophers(t_philo *philo);

#endif