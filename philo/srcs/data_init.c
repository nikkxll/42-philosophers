/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:07:14 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 22:45:21 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void init_philosopher(t_shared *shared, int i)
{
	shared->philo[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
	if (!shared->philo[i])
		return ;
	shared->philo[i]->shared = shared;
	shared->philo[i]->philo_id = i;
}

int	data_initialization(t_shared *shared)
{
	shared->philo = (t_philo **)ft_calloc(shared->input->num_of_philo, sizeof(t_philo *));
	if (!shared->philo)
		return (error_struct_free(shared, "Malloc error\n"));
	for (int i = 0; i < shared->input->num_of_philo; i++)
        init_philosopher(shared, i);
	shared->flag_locker = 1;
	shared->fork_mutex = (t_mutex *)
		ft_calloc(shared->input->num_of_philo, sizeof(t_mutex));
	if (!shared->fork_mutex)
		return (error_struct_free(shared, "Malloc error\n"));
	shared->meals = (int *)
		ft_calloc(shared->input->num_of_philo, sizeof(int));
	if (!shared->meals)
		return (error_struct_free(shared, "Malloc error\n"));
	shared->last_meal_ts = (long long *)
		ft_calloc(shared->input->num_of_philo, sizeof(long long));
	if (!shared->last_meal_ts)
		return (error_struct_free(shared, "Malloc error\n"));
	return (0);
}
