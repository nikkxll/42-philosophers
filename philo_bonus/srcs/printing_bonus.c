/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 18:03:54 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 18:29:30 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

void	print_message(int procedure, t_shared *shared)
{
	sem_wait(shared->locker);
	if (procedure == THINK)
		printf("%lld %d is thinking\n", get_timestamp(shared),
			shared->philo_id + 1);
	else if (procedure == EAT)
	{
		printf("%lld %d has taken a fork\n", get_timestamp(shared),
			shared->philo_id + 1);
		printf("%lld %d has taken a fork\n", get_timestamp(shared),
			shared->philo_id + 1);
		printf("%lld %d is eating\n", get_timestamp(shared),
			shared->philo_id + 1);
	}
	else if (procedure == SLEEP)
		printf("%lld %d is sleeping\n", get_timestamp(shared),
			shared->philo_id + 1);
	sem_post(shared->locker);
}
