/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 16:18:16 by dnikifor         ###   ########.fr       */
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

typedef struct s_shared
{
	long		num_of_philo;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		eat_number;
	int			args_number;
	int			flag_locker;
	int			*meals;
	long long	*last_meal_ts;
	t_mutex		locker;
	t_mutex		meal;
	t_mutex		*fork_mutex;
	t_time		tv;
	pthread_t	monitor;
	t_philo		**philo;
}	t_shared;


typedef struct s_philo
{
	t_shared	*shared;
	pthread_t	philo_pth;
	int			philo_id;
}	t_philo;


long	ft_atol(const char *str);
int		reader(int ac, char **av, t_shared *shared);
int		error_msg(char *msg);
int		error_struct_free(t_shared *shared, char *message);
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t count, size_t size);
int		initialization(int ac, char **av, t_shared *shared);
int		data_initialization(t_shared *shared);
int		philosophers(t_shared *shared);

#endif