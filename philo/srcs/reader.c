/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:13:59 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 12:02:04 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	is_valid(int c)
{
	if (((c >= 48) && (c <= 57)) || (c == ' ' || c == '+' || c == '\t'))
		return (0);
	else
		return (-1);
}

static int	check_arg(int ac, char **av, int i, int j)
{
	while (i++ < ac - 1)
	{
		j = 0;
		while (av[i][j])
		{
			if (is_valid(av[i][j]) == -1)
				return (-1);
			j++;
		}
	}
	return (0);
}

int	reader(int ac, char **av, t_shared *shared)
{
	if (ac < 5 || ac > 6)
		return (error_struct_free(shared, "Args number error.\n"));
	if (check_arg(ac, av, 0, 0) == -1)
		return (error_struct_free(shared, "Args content error.\n"));
	shared->args_number = ac - 1;
	shared->num_of_philo = ft_atol(av[1]);
	shared->time_to_die = ft_atol(av[2]) * 1e3;
	shared->time_to_eat = ft_atol(av[3]) * 1e3;
	shared->time_to_sleep = ft_atol(av[4]) * 1e3;
	if (shared->num_of_philo == -1 || shared->num_of_philo == 0
		|| shared->time_to_die == -1e3 || shared->time_to_die == 0
		|| shared->time_to_eat == -1e3 || shared->time_to_eat == 0
		|| shared->time_to_sleep == -1e3
		|| shared->time_to_sleep == 0)
		return (error_struct_free(shared, "Args convertion error.\n"));
	if (ac == 6)
	{
		shared->eat_number = ft_atol(av[5]);
		if (shared->eat_number == -1)
			return (error_struct_free(shared, "Args convertion error.\n"));
	}
	else
		shared->eat_number = 0;
	return (0);
}
