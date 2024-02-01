/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 17:40:51 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 15:20:15 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

long long	get_timestamp(t_shared *shared)
{
	if (gettimeofday(&shared->tv, NULL) == -1)
	{
		sem_wait(shared->locker);
		error_msg("gettimeofday error\n");
		semaphore_post_control(shared);
	}
	else
		return ((long long)shared->tv.tv_sec * 1000
			+ (long long)shared->tv.tv_usec / 1000);
	return (0);
}

void	ft_usleep(int ms, t_shared *shared)
{
	long	start_time;
	long	end_time;

	start_time = get_timestamp(shared);
	end_time = start_time + ms;
	while (get_timestamp(shared) < end_time)
	{
		if (usleep(ms / 10) == -1)
		{
			sem_wait(shared->locker);
			error_msg("usleep error\n");
			semaphore_post_control(shared);
		}
	}
}

int	semaphore_wait_control(t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		if (sem_wrapper(shared->end, WAIT))
			return (1);
	}
	return (0);
}

int	semaphore_post_control(t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		if (sem_wrapper(shared->end, POST))
			return (1);
	}
	return (0);
}

void	process_error(t_shared *shared, char *msg)
{
	sem_wait(shared->locker);
	error_msg(msg);
	semaphore_post_control(shared);
}
