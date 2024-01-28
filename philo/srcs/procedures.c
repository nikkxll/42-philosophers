/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedures.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:23:25 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/28 14:24:19 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	ft_sleep_delay(int procedure, t_philo *philo)
{
	mutex_wrapper(&philo->shared->locker, LOCK);
	if (procedure == EAT && philo->shared->flag_locker)
	{
		mutex_wrapper(&philo->shared->locker, UNLOCK);
		ft_usleep(philo->shared->input->time_to_eat, philo);
	}
	else if (procedure == SLEEP && philo->shared->flag_locker)
	{
		mutex_wrapper(&philo->shared->locker, UNLOCK);
		ft_usleep(philo->input->time_to_sleep, philo);
	}
	else
		mutex_wrapper(&philo->shared->locker, UNLOCK);
}

void	ft_eating(t_philo *philo, int left, int right)
{
	if (right < left)
	{
		mutex_wrapper(&philo->shared->fork_mutex[right], LOCK);
		mutex_wrapper(&philo->shared->fork_mutex[left], LOCK);
	}
	else
	{
		mutex_wrapper(&philo->shared->fork_mutex[left], LOCK);
		mutex_wrapper(&philo->shared->fork_mutex[right], LOCK);
	}
	print_message(EAT, philo);
	meal_timestamp(philo);
	ft_sleep_delay(EAT, philo);
	mutex_wrapper(&philo->shared->fork_mutex[right], UNLOCK);
	mutex_wrapper(&philo->shared->fork_mutex[left], UNLOCK);
	mutex_wrapper(&philo->shared->meal, LOCK);
	philo->shared->meals[philo->philo_id]++;
	mutex_wrapper(&philo->shared->meal, UNLOCK);
}
