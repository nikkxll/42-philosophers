/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:39:31 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 09:46:40 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

int	error_struct_free(t_shared *shared, char *message)
{
	// if (philo->meals)
	// 	free(philo->meals);
	// if (philo->last_meal_ts)
	// 	free(philo->last_meal_ts);
	// if (philo->philo_ids)
	// 	free(philo->philo_ids);
	// if (philo->philosophers)
	// 	free(philo->philosophers);
	// if (philo->fork_mutex)
	// 	free(philo->fork_mutex);
	// free(philo->input);
	free(shared);
	return (error_msg(message));
}
