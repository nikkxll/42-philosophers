/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 23:01:02 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	meals_checker(int *arr, int size, int eats)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (arr[i] >= eats)
		{
			i++;
			continue ;
		}
		else
			return (0);
	}
	return (1);
}

long long	get_timestamp(t_philo *philo)
{
	gettimeofday(&philo->shared->tv, NULL);
	return ((long long)philo->shared->tv.tv_sec * 1000
		+ (long long)philo->shared->tv.tv_usec / 1000);
}

void	ft_usleep(int ms, t_philo *philo)
{
	long	start_time;
	long	end_time;

	start_time = get_timestamp(philo);
	end_time = start_time + ms;
	while (get_timestamp(philo) < end_time)
		usleep(ms / 10);
}

int	meals_finish_check(t_shared *shared)
{
	if (shared->input->eat_number
		&& meals_checker(shared->meals, shared->input->num_of_philo,
			shared->input->eat_number))
	{
		pthread_mutex_unlock(&shared->meal);
		pthread_mutex_lock(&shared->locker);
		shared->flag_locker = 0;
		pthread_mutex_unlock(&shared->locker);
		return (1);
	}
	return (0);
}

int	death_check(t_shared *shared, int j)
{
	long long	current_timestamp;

	current_timestamp = get_timestamp(shared->philo[j]);
	if (current_timestamp - shared->last_meal_ts[j]
		>= shared->input->time_to_die
		&& shared->last_meal_ts[j])
	{
		pthread_mutex_unlock(&shared->meal);
		pthread_mutex_lock(&shared->locker);
		shared->flag_locker = 0;
		pthread_mutex_unlock(&shared->locker);
		pthread_mutex_lock(&shared->meal);
		current_timestamp = get_timestamp(shared->philo[j]);
		printf(RED_COLOR "%lld %d died\n" RESET_COLOR,
			current_timestamp, j + 1);
		pthread_mutex_unlock(&shared->meal);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	int			j;
	t_shared	*shared;

	shared = (t_shared *)arg;
	while (1)
	{
		pthread_mutex_lock(&shared->meal);
		if (meals_finish_check(shared))
			return (NULL);
		pthread_mutex_unlock(&shared->meal);
		j = -1;
		while (++j < shared->input->num_of_philo)
		{
			pthread_mutex_lock(&shared->meal);
			if (death_check(shared, j))
				return (NULL);
			pthread_mutex_unlock(&shared->meal);
		}
	}
}

void	print_message(int procedure, t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->locker);
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
	pthread_mutex_unlock(&philo->shared->locker);
}

void	ft_sleep_delay(int procedure, t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->locker);
	if (procedure == EAT && philo->shared->flag_locker)
	{
		pthread_mutex_unlock(&philo->shared->locker);
		ft_usleep(philo->shared->input->time_to_eat, philo);
	}
	else if (procedure == SLEEP && philo->shared->flag_locker)
	{
		pthread_mutex_unlock(&philo->shared->locker);
		ft_usleep(philo->input->time_to_sleep, philo);
	}
	else
		pthread_mutex_unlock(&philo->shared->locker);
}

void	meal_timestamp(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->meal);
	philo->shared->last_meal_ts[philo->philo_id] = get_timestamp(philo);
	pthread_mutex_unlock(&philo->shared->meal);
}

void	ft_eating(t_philo *philo, int left, int right)
{
	if (right < left)
	{
		pthread_mutex_lock(&philo->shared->fork_mutex[right]);
		pthread_mutex_lock(&philo->shared->fork_mutex[left]);
	}
	else
	{
		pthread_mutex_lock(&philo->shared->fork_mutex[left]);
		pthread_mutex_lock(&philo->shared->fork_mutex[right]);
	}
	print_message(EAT, philo);
	meal_timestamp(philo);
	ft_sleep_delay(EAT, philo);
	pthread_mutex_unlock(&philo->shared->fork_mutex[right]);
	pthread_mutex_unlock(&philo->shared->fork_mutex[left]);
	pthread_mutex_lock(&philo->shared->meal);
	philo->shared->meals[philo->philo_id]++;
	pthread_mutex_unlock(&philo->shared->meal);
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
	pthread_mutex_lock(&philo->shared->locker);
	while (philo->shared->flag_locker)
	{
		pthread_mutex_unlock(&philo->shared->locker);
		print_message(THINK, philo);
		ft_eating(philo, left, right);
		print_message(SLEEP, philo);
		ft_sleep_delay(SLEEP, philo);
		pthread_mutex_lock(&philo->shared->locker);
	}
	pthread_mutex_unlock(&philo->shared->locker);
	return (NULL);
}

int	mutex_cleaner(t_shared *shared, int current, int i)
{
	while (++i < current)
		mutex_wrapper(&shared->fork_mutex[i], DESTROY);
	mutex_wrapper(&shared->locker, DESTROY);
	mutex_wrapper(&shared->meal, DESTROY);	
	return (struct_free(shared, NULL, 0));
}

void	thread_cleaner(t_shared *shared, int current, int i)
{
	while (++i < current)
		thread_wrapper(&shared->philo[i]->philo_pth, NULL, NULL, JOIN);
}

int	philosophers(t_shared *shared, int i, int error)
{
	if (mutex_wrapper(&shared->locker, INIT))
		return (struct_free(shared, NULL, 0));
	if (mutex_wrapper(&shared->meal, INIT))
	{
		mutex_wrapper(&shared->locker, DESTROY);
		return (struct_free(shared, NULL, 0));
	}
	while (++i < shared->input->num_of_philo)
	{
		if (mutex_wrapper(&shared->fork_mutex[i], INIT))
			return (mutex_cleaner(shared, i, -1));
	}
	if (thread_wrapper(&shared->monitor, monitor, (void *)shared, CREATE))
		return (mutex_cleaner(shared, shared->input->num_of_philo, -1));
	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		if (thread_wrapper(&shared->philo[i]->philo_pth,
			philo_routine, (void *)shared->philo[i], CREATE))
		{
			thread_wrapper(&shared->monitor, NULL, NULL, JOIN);
			thread_cleaner(shared, i, -1);
			return (mutex_cleaner(shared, shared->input->num_of_philo, -1));
		}
	}
	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		if (thread_wrapper(&shared->philo[i]->philo_pth, NULL, NULL, JOIN))
			error = 1;
	}
	if (thread_wrapper(&shared->monitor, NULL, NULL, JOIN))
		error = 1;
	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		if (mutex_wrapper(&shared->fork_mutex[i], DESTROY))
			error = 1;
	}
	if (mutex_wrapper(&shared->locker, DESTROY))
		error = 1;
	if (mutex_wrapper(&shared->meal, DESTROY))
		error = 1;
	return (error);
}
