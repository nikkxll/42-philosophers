/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:21:27 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/28 14:21:47 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_message(int procedure, t_philo *philo)
{
	mutex_wrapper(&philo->shared->locker, LOCK);
	if (procedure == THINK && philo->shared->flag_locker)
		printf("%lld %d is thinking\n", get_timestamp(philo),
			philo->philo_id + 1);
	else if (procedure == EAT && philo->shared->flag_locker)
	{
		printf("%lld %d has taken a fork\n", get_timestamp(philo),
			philo->philo_id + 1);
		printf("%lld %d has taken a fork\n", get_timestamp(philo),
			philo->philo_id + 1);
		printf("%lld %d is eating\n", get_timestamp(philo),
			philo->philo_id + 1);
	}
	else if (procedure == SLEEP && philo->shared->flag_locker)
		printf("%lld %d is sleeping\n", get_timestamp(philo),
			philo->philo_id + 1);
	mutex_wrapper(&philo->shared->locker, UNLOCK);
}
