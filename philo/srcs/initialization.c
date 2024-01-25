/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:52:48 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/25 18:07:25 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	initialization(int ac, char **av, t_philo *philo)
{
	philo->input = (t_input *)ft_calloc(1, sizeof(t_input));
	if (!philo->input)
	{
		free(philo);
		return (error_msg("Malloc error\n"));
	}
	if (reader(ac, av, philo))
		return (1);
	if (data_initialization(philo))
		return (1);
	return (0);
}
