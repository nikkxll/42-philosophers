/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:07:14 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 22:59:17 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	init_philosopher(t_shared *shared, t_input *input, int i)
{
	shared->philo[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
	if (!shared->philo[i])
		return (1);
	shared->philo[i]->shared = shared;
	shared->philo[i]->input = input;
	shared->philo[i]->philo_id = i;
	return (0);
}

int	data_initialization(t_shared *shared, int i)
{
	shared->philo = (t_philo **)ft_calloc(shared->input->num_of_philo, sizeof(t_philo *));
	if (!shared->philo)
		return (struct_free(shared, "Malloc error\n", 1));
	while (++i < shared->input->num_of_philo)
	{
		if (init_philosopher(shared, shared->input, i))
			return (struct_free(shared, "Malloc error\n", 1));
	}
	shared->flag_locker = 1;
	shared->exit_flag = 0;
	shared->fork_mutex = (t_mutex *)
		ft_calloc(shared->input->num_of_philo, sizeof(t_mutex));
	if (!shared->fork_mutex)
		return (struct_free(shared, "Malloc error\n", 1));
	shared->meals = (int *)
		ft_calloc(shared->input->num_of_philo, sizeof(int));
	if (!shared->meals)
		return (struct_free(shared, "Malloc error\n", 1));
	shared->last_meal_ts = (long long *)
		ft_calloc(shared->input->num_of_philo, sizeof(long long));
	if (!shared->last_meal_ts)
		return (struct_free(shared, "Malloc error\n", 1));
	return (0);
}
