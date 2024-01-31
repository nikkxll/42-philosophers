/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:52:48 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 19:15:46 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

int	semaphore_clearing(t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < shared->input->num_of_philo)
		kill(shared->pid[i], SIGINT);
	if (sem_wrapper(shared->death, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	if (sem_wrapper(shared->end, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	if (sem_wrapper(shared->forks, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	if (sem_wrapper(shared->locker, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	sem_unlink("/forks");
	sem_unlink("/locker");
	sem_unlink("/end");
	sem_unlink("/death");
	return (0);
}

static int	data_initialization(t_shared *shared)
{
	sem_unlink("/forks");
	sem_unlink("/locker");
	sem_unlink("/end");
	sem_unlink("/death");
	shared->forks = sem_open("/forks", O_CREAT, 0644,
			shared->input->num_of_philo);
	if (!shared->forks)
		return (struct_free(shared, "Semaphore creation error\n", 1));
	shared->locker = sem_open("/locker", O_CREAT, 0644, 1);
	if (!shared->locker)
		return (struct_free(shared, "Semaphore creation error\n", 1));
	shared->end = sem_open("/end", O_CREAT, 0644, shared->input->num_of_philo);
	if (!shared->end)
		return (struct_free(shared, "Semaphore creation error\n", 1));
	shared->death = sem_open("/death", O_CREAT, 0644, 1);
	if (!shared->death)
		return (struct_free(shared, "Semaphore creation error\n", 1));
	shared->meals = 0;
	shared->last_meal_ts = 0;
	shared->philo_id = 0;
	shared->pid = (pid_t *)
		ft_calloc(shared->input->num_of_philo, sizeof(pid_t));
	if (!shared->pid)
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
	if (data_initialization(shared))
		return (1);
	return (0);
}
