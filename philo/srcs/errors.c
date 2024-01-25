/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:39:31 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/25 18:09:00 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

int	error_struct_free(t_philo *philo, char *message)
{
	if (philo->meals)
		free(philo->meals);
	if (philo->last_meal_ts)
		free(philo->last_meal_ts);
	if (philo->philo_ids)
		free(philo->philo_ids);
	if (philo->philosophers)
		free(philo->philosophers);
	if (philo->fork_mutex)
		free(philo->fork_mutex);
	free(philo->input);
	free(philo);
	return (error_msg(message));
}
