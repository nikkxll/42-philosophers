/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/28 14:24:16 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*monitor(void *arg)
{
	int			j;
	t_shared	*shared;

	shared = (t_shared *)arg;
	mutex_wrapper(&shared->locker, LOCK);
	while (shared->flag_locker)
	{
		mutex_wrapper(&shared->locker, UNLOCK);
		mutex_wrapper(&shared->meal, LOCK);
		if (meals_finish_check(shared))
			return (NULL);
		mutex_wrapper(&shared->meal, UNLOCK);
		j = -1;
		while (++j < shared->input->num_of_philo)
		{
			mutex_wrapper(&shared->meal, LOCK);
			if (death_check(shared, j))
				return (NULL);
			mutex_wrapper(&shared->meal, UNLOCK);
		}
		mutex_wrapper(&shared->locker, LOCK);
	}
	mutex_wrapper(&shared->locker, UNLOCK);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		right;
	int		left;

	philo = (t_philo *)arg;
	right = philo->philo_id;
	left = (philo->philo_id + philo->input->num_of_philo - 1)
		% philo->input->num_of_philo;
	mutex_wrapper(&philo->shared->locker, LOCK);
	while (philo->shared->flag_locker)
	{
		mutex_wrapper(&philo->shared->locker, UNLOCK);
		print_message(THINK, philo);
		ft_eating(philo, left, right);
		print_message(SLEEP, philo);
		ft_sleep_delay(SLEEP, philo);
		mutex_wrapper(&philo->shared->locker, LOCK);
	}
	mutex_wrapper(&philo->shared->locker, UNLOCK);
	return (NULL);
}

int	routine_creating(t_shared *shared, int i, int j)
{
	if (mutex_wrapper(&shared->locker, INIT))
		return (struct_free(shared, NULL, 2));
	if (mutex_wrapper(&shared->meal, INIT))
	{
		mutex_wrapper(&shared->locker, DESTROY);
		return (struct_free(shared, NULL, 2));
	}
	while (++i < shared->input->num_of_philo)
	{
		if (mutex_wrapper(&shared->fork_mutex[i], INIT))
			return (mutex_cleaner(shared, i, -1));
	}
	if (thread_wrapper(&shared->monitor, monitor, (void *)shared, CREATE))
		return (mutex_cleaner(shared, shared->input->num_of_philo, -1));
	while (++j < shared->input->num_of_philo)
	{
		if (thread_wrapper(&shared->philo[j]->philo_pth,
			philo_routine, (void *)shared->philo[j], CREATE))
		{
			flag_locker(shared, 0);
			return (mutex_cleaner(shared, shared->input->num_of_philo, -1));
		}
		usleep(10);
	}
	return (0);
}

int	routine_ending(t_shared *shared, int i)
{
	while (++i < shared->input->num_of_philo)
	{
		if (thread_wrapper(&shared->philo[i]->philo_pth, NULL, NULL, JOIN))
			return (mutex_cleaner(shared, shared->input->num_of_philo, -1));
	}
	if (thread_wrapper(&shared->monitor, NULL, NULL, JOIN))
		return (mutex_cleaner(shared, shared->input->num_of_philo, -1));
	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		if (mutex_wrapper(&shared->fork_mutex[i], DESTROY))
			return (mutex_cleaner(shared, shared->input->num_of_philo, i));
	}
	if (mutex_wrapper(&shared->locker, DESTROY))
	{
		mutex_wrapper(&shared->meal, DESTROY);
		return (struct_free(shared, NULL, 2));
	}
	if (mutex_wrapper(&shared->meal, DESTROY))
		return (struct_free(shared, NULL, 2));
	if (shared->exit_flag)
		return (struct_free(shared, NULL, 2));
	return (0);
}

int	philosophers(t_shared *shared)
{
	if (routine_creating(shared, -1, -1))
		return (1);
	return (routine_ending(shared, -1));
}
