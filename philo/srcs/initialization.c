/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:52:48 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/28 14:49:13 by dnikifor         ###   ########.fr       */
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
	shared->philo = (t_philo **)ft_calloc(shared->input->num_of_philo + 1, sizeof(t_philo *));
	if (!shared->philo)
		return (struct_free(shared, "Malloc error\n", 1));
	while (++i < shared->input->num_of_philo)
	{
		if (init_philosopher(shared, shared->input, i))
			return (struct_free(shared, "Malloc error\n", 1));
	}
	shared->philo[shared->input->num_of_philo] = NULL;
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

int	initialization(int ac, char **av, t_shared *shared)
{
	shared->input = (t_input *)ft_calloc(1, sizeof(t_input));
	if (!shared->input)
		return (struct_free(shared, "Malloc error\n", 1));
	if (reader(ac, av, shared))
		return (1);
	if (data_initialization(shared, -1))
		return (1);
	return (0);
}
