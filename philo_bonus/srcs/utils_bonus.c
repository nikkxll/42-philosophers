/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 17:40:51 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 18:45:21 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

long long	get_timestamp(t_shared *shared)
{
	if (gettimeofday(&shared->tv, NULL) == -1)
		return (error_msg("gettimeofday error\n"));
	else
		return ((long long)shared->tv.tv_sec * 1000
			+ (long long)shared->tv.tv_usec / 1000);
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
			error_msg("usleep error\n");
			return ;
		}
	}
}

void	semaphore_parent_control(t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < shared->input->num_of_philo)
		sem_wait(shared->end);
}
