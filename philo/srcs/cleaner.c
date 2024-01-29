/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:39:31 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/29 17:15:12 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	error_msg(char *msg)
{
	write(2, RED_COLOR, sizeof(RED_COLOR));
	write(2, msg, ft_strlen(msg));
	write(2, RESET_COLOR, sizeof(RESET_COLOR));
	return (1);
}

static void	arrays_free(t_shared *shared)
{
	if (shared->meals)
		free(shared->meals);
	if (shared->last_meal_ts)
		free(shared->last_meal_ts);
	if (shared->fork_mutex)
		free(shared->fork_mutex);
}

int	struct_free(t_shared *shared, char *message, int status)
{
	t_philo	**philo;

	philo = shared->philo;
	arrays_free(shared);
	if (shared->philo)
	{
		while (*philo)
		{
			free(*philo);
			philo++;
		}
	}
	if (shared->philo)
		free(shared->philo);
	if (shared->input)
		free(shared->input);
	free(shared);
	if (status == 1)
		return (error_msg(message));
	else if (status == 2)
		return (1);
	return (0);
}

int	mutex_cleaner(t_shared *shared, int current, int i)
{
	while (++i < current)
		mutex_wrapper(&shared->fork_mutex[i], DESTROY);
	mutex_wrapper(&shared->locker, DESTROY);
	mutex_wrapper(&shared->meal, DESTROY);
	return (struct_free(shared, NULL, 2));
}
