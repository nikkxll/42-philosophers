/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:07:14 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/25 18:55:22 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	data_initialization(t_philo *philo)
{
	philo->meals = (int *)ft_calloc(philo->input->num_of_philo, sizeof(int));
	if (!philo->meals)
		return (error_struct_free(philo, "Malloc error\n"));
	philo->last_meal_ts = (long long *)
		ft_calloc(philo->input->num_of_philo, sizeof(long long));
	if (!philo->last_meal_ts)
		return (error_struct_free(philo, "Malloc error\n"));
	philo->philo_ids = (int *)
		ft_calloc(philo->input->num_of_philo, sizeof(int));
	if (!philo->philo_ids)
		return (error_struct_free(philo, "Malloc error\n"));
	philo->philosophers = (pthread_t *)
		ft_calloc(philo->input->num_of_philo, sizeof(pthread_t));
	if (!philo->philosophers)
		return (error_struct_free(philo, "Malloc error\n"));
	philo->fork_mutex = (t_mutex *)
		ft_calloc(philo->input->num_of_philo, sizeof(t_mutex));
	if (!philo->fork_mutex)
		return (error_struct_free(philo, "Malloc error\n"));
	memset(philo->meals, 0, philo->input->num_of_philo * sizeof(int));
	memset(philo->last_meal_ts, 0, philo->input->num_of_philo
		* sizeof(long long));
	philo->flag_locker = 1;
	return (0);
}
