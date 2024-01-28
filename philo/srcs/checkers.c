/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:22:30 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/28 22:06:48 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	meals_finish_check(t_shared *shared)
{
	if (shared->input->solo && shared->input->eat_number
		&& meals_checker(shared->meals, shared->input->num_of_philo,
			shared->input->eat_number))
	{
		mutex_wrapper(&shared->meal, UNLOCK);
		flag_locker(shared, 0);
		return (1);
	}
	return (0);
}

int	death_check(t_shared *shared, int j)
{
	long long	current_timestamp;

	current_timestamp = get_timestamp(shared->philo[j]);
	if (current_timestamp - shared->last_meal_ts[j]
		>= shared->input->time_to_die
		&& shared->last_meal_ts[j])
	{
		mutex_wrapper(&shared->meal, UNLOCK);
		flag_locker(shared, 0);
		mutex_wrapper(&shared->meal, LOCK);
		current_timestamp = get_timestamp(shared->philo[j]);
		printf(RED_COLOR "%lld %d died\n" RESET_COLOR,
			current_timestamp, j + 1);
		mutex_wrapper(&shared->meal, UNLOCK);
		return (1);
	}
	return (0);
}
