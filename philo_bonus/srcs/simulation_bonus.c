/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 15:25:02 by dnikifor         ###   ########.fr       */
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
		wait_post_wrapper(shared, shared->death, WAIT);
		current_timestamp = get_timestamp(shared);
		if (current_timestamp - shared->last_meal_ts
			>= shared->input->time_to_die && shared->last_meal_ts)
		{
			wait_post_wrapper(shared, shared->locker, WAIT);
			printf(RED_COLOR "%lld %d died\n" RESET_COLOR, current_timestamp,
				shared->philo_id + 1);
			while (++i < shared->input->num_of_philo)
				wait_post_wrapper(shared, shared->end, POST);
			return (NULL);
		}
		wait_post_wrapper(shared, shared->death, POST);
	}
}

static void	ft_eating(t_shared *shared)
{
	wait_post_wrapper(shared, shared->forks, WAIT);
	wait_post_wrapper(shared, shared->forks, WAIT);
	wait_post_wrapper(shared, shared->locker, WAIT);
	print_message(EAT, shared);
	wait_post_wrapper(shared, shared->locker, POST);
	wait_post_wrapper(shared, shared->death, WAIT);
	shared->last_meal_ts = get_timestamp(shared);
	wait_post_wrapper(shared, shared->death, POST);
	ft_usleep(shared->input->time_to_eat, shared);
	wait_post_wrapper(shared, shared->locker, WAIT);
	shared->meals++;
	if (shared->meals == shared->input->eat_number)
		wait_post_wrapper(shared, shared->end, POST);
	wait_post_wrapper(shared, shared->locker, POST);
	wait_post_wrapper(shared, shared->forks, POST);
	wait_post_wrapper(shared, shared->forks, POST);
}

static void	routine(t_shared *shared)
{
	if (thread_wrapper(&shared->monitor, monitor, (void *)shared, CREATE))
		semaphore_post_control(shared);
	if (thread_wrapper(&shared->monitor, monitor, (void *)shared, DETACH))
		semaphore_post_control(shared);
	while (1)
	{
		wait_post_wrapper(shared, shared->locker, WAIT);
		print_message(THINK, shared);
		wait_post_wrapper(shared, shared->locker, POST);
		ft_eating(shared);
		wait_post_wrapper(shared, shared->locker, WAIT);
		print_message(SLEEP, shared);
		wait_post_wrapper(shared, shared->locker, POST);
		ft_usleep(shared->input->time_to_sleep, shared);
	}
}

static int	child_creation(t_shared *shared, int i, int j)
{
	while (++i < shared->input->num_of_philo)
	{
		shared->philo_id = i;
		shared->pid[i] = fork();
		if (shared->pid[i] == -1)
		{
			semaphore_clearing(shared, 1, j, i);
			struct_free(shared, "fork error\n", 1);
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
	if (semaphore_wait_control(shared))
	{
		semaphore_clearing(shared, 0, 0, 0);
		struct_free(shared, NULL, 2);
	}
	child_creation(shared, -1, -1);
	if (semaphore_wait_control(shared))
	{
		semaphore_clearing(shared, 1, -1, shared->input->num_of_philo);
		struct_free(shared, NULL, 2);
	}
	semaphore_clearing(shared, 1, -1, shared->input->num_of_philo);
	struct_free(shared, NULL, 0);
	return (0);
}
