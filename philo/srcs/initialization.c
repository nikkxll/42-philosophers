/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:52:48 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/18 23:41:02 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	initialization(int ac, char **av, t_philo *philo)
{
	philo->input = (t_input *)malloc(sizeof(t_input));
	if (!philo->input)
		return (-1);
	if (reader(ac, av, philo->input) == -1)
		return (-1);
	if (data_initialization(philo) == -1)
		return (-1);
	return (0);
}
