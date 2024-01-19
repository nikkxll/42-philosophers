/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:13:59 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/19 14:43:31 by dnikifor         ###   ########.fr       */
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

int	reader(int ac, char **av, t_input *input)
{
	if (ac < 5 || ac > 6)
		return (error_msg("Args number error.\n"));
	if (check_arg(ac, av, 0, 0) == -1)
		return (error_msg("Args content error.\n"));
	input->args_number = ac - 1;
	input->num_of_philo = ft_atol(av[1]);
	input->time_to_die = ft_atol(av[2]) * 1e3;
	input->time_to_eat = ft_atol(av[3]) * 1e3;
	input->time_to_sleep = ft_atol(av[4]) * 1e3;
	if (input->num_of_philo == -1 || input->num_of_philo == 0
		|| input->time_to_die == -1 * 1e3 || input->time_to_die == 0
		|| input->time_to_eat == -1 * 1e3 || input->time_to_eat == 0
		|| input->time_to_sleep == -1 * 1e3 || input->time_to_sleep == 0)
		return (error_msg("Args ft_atol convertion error.\n"));
	if (ac == 6)
	{
		input->eat_number = ft_atol(av[5]);
		if (input->eat_number == -1 || input->eat_number == 0)
			return (error_msg("Args ft_atol convertion error.\n"));
	}
	else
		input->eat_number = -1;
	return (0);
}
