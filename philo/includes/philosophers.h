/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:08:02 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/18 19:57:58 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
#include <stdio.h> //

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
	t_input	*input;
}	t_philo;

long	ft_atol(const char *str);
int		reader(int ac, char **av, t_input *input);
int		error_msg(char *msg, t_philo *philo);
size_t	ft_strlen(const char *s);
int		input_init(int ac, char **av, t_philo *philo);

#endif