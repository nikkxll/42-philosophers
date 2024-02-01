/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 18:03:54 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 15:25:20 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

void	print_message(int procedure, t_shared *shared)
{
	if (procedure == THINK)
	{
		if (printf("%lld %d is thinking\n", get_timestamp(shared),
				shared->philo_id + 1) < 0)
			process_error(shared, "printf error\n");
	}
	else if (procedure == EAT)
	{
		if (printf("%lld %d has taken a fork\n", get_timestamp(shared),
				shared->philo_id + 1) < 0)
			process_error(shared, "printf error\n");
		if (printf("%lld %d has taken a fork\n", get_timestamp(shared),
				shared->philo_id + 1) < 0)
			process_error(shared, "printf error\n");
		if (printf("%lld %d is eating\n", get_timestamp(shared),
				shared->philo_id + 1) < 0)
			process_error(shared, "printf error\n");
	}
	else
	{
		if (printf("%lld %d is sleeping\n", get_timestamp(shared),
				shared->philo_id + 1) < 0)
			process_error(shared, "printf error\n");
	}
}
