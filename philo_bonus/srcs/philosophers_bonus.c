/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 19:16:13 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

static void	*monitor(void *arg)
{
	t_shared	*shared;
	long long	current_timestamp;
	int			i;

	i = -1;
	shared = (t_shared *)arg;
	current_timestamp = get_timestamp(shared);
	while (1)
	{
		sem_wrapper(shared->death, WAIT);
		current_timestamp = get_timestamp(shared);
		if (current_timestamp - shared->last_meal_ts
			>= shared->input->time_to_die && shared->last_meal_ts)
		{
			sem_wrapper(shared->locker, WAIT);
			printf(RED_COLOR "%lld %d died\n" RESET_COLOR, current_timestamp,
				shared->philo_id + 1);
			while (++i < shared->input->num_of_philo)
				sem_wrapper(shared->end, POST);
			return (NULL);
		}
		sem_wrapper(shared->death, POST);
	}
}

void	ft_eating(t_shared *shared)
{
	sem_wrapper(shared->forks, WAIT);
	sem_wrapper(shared->forks, WAIT);
	print_message(EAT, shared);
	sem_wrapper(shared->death, WAIT);
	shared->last_meal_ts = get_timestamp(shared);
	sem_wrapper(shared->death, POST);
	ft_usleep(shared->input->time_to_eat, shared);
	sem_wrapper(shared->locker, WAIT);
	shared->meals++;
	if (shared->meals == shared->input->eat_number)
		sem_wrapper(shared->end, POST);
	sem_wrapper(shared->locker, POST);
	sem_wrapper(shared->forks, POST);
	sem_wrapper(shared->forks, POST);
}

void	routine(t_shared *shared)
{
	thread_wrapper(&shared->monitor, monitor, (void *)shared, CREATE);
	thread_wrapper(&shared->monitor, monitor, (void *)shared, DETACH);
	while (1)
	{
		print_message(THINK, shared);
		ft_eating(shared);
		print_message(SLEEP, shared);
		ft_usleep(shared->input->time_to_sleep, shared);
	}
}

static int	child_creation(t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		shared->philo_id = i;
		shared->pid[i] = fork();
		if (shared->pid[i] == -1)
		{
			error_msg("fork error\n");
			exit(EXIT_FAILURE);
		}
		if (shared->pid[i] == 0)
		{
			routine(shared);
			exit(EXIT_SUCCESS);
		}
	}
	return (0);
}

int	philosophers(t_shared *shared)
{
	semaphore_parent_control(shared);
	child_creation(shared);
	semaphore_parent_control(shared);
	semaphore_clearing(shared);
	return (0);
}
