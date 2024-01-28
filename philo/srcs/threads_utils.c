/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:17:29 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/28 21:57:38 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	flag_locker(t_shared *shared, int status)
{
	if (status == 1)
	{
		mutex_wrapper(&shared->locker, LOCK);
		shared->flag_locker = 0;
		shared->exit_flag = 1;
		mutex_wrapper(&shared->locker, UNLOCK);
	}
	else if (status == 0)
	{
		mutex_wrapper(&shared->locker, LOCK);
		shared->flag_locker = 0;
		mutex_wrapper(&shared->locker, UNLOCK);
	}
	else
	{
		mutex_wrapper(&shared->locker, LOCK);
		shared->input->solo = 0;
		mutex_wrapper(&shared->locker, UNLOCK);
	}
}

int	meals_checker(int *arr, int size, int eats)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (arr[i] >= eats)
		{
			i++;
			continue ;
		}
		else
			return (0);
	}
	return (1);
}

void	meal_timestamp(t_philo *philo)
{
	mutex_wrapper(&philo->shared->meal, LOCK);
	philo->shared->last_meal_ts[philo->philo_id] = get_timestamp(philo);
	mutex_wrapper(&philo->shared->meal, UNLOCK);
}

long long	get_timestamp(t_philo *philo)
{
	if (gettimeofday(&philo->shared->tv, NULL) == -1)
	{
		flag_locker(philo->shared, 1);
		return (error_msg("gettimeofday error\n"));
	}
	else
		return ((long long)philo->shared->tv.tv_sec * 1000
			+ (long long)philo->shared->tv.tv_usec / 1000);
}

void	ft_usleep(int ms, t_philo *philo)
{
	long	start_time;
	long	end_time;

	start_time = get_timestamp(philo);
	end_time = start_time + ms;
	while (get_timestamp(philo) < end_time)
	{
		if (usleep(ms / 10) == -1)
		{
			flag_locker(philo->shared, 1);
			error_msg("usleep error\n");
			return ;
		}
	}
}
