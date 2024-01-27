/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 16:56:56 by dnikifor         ###   ########.fr       */
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

void	ft_sleep(int procedure, t_philo *philo)
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

void	fork_locker(t_philo *philo, int left, int right)
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
}

void	meal_timestamp(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->meal);
	philo->shared->last_meal_ts[philo->philo_id] = get_timestamp(philo);
	pthread_mutex_unlock(&philo->shared->meal);
}

void	simulation(t_philo *philo, int left, int right)
{
	pthread_mutex_unlock(&philo->shared->locker);
	print_message(THINK, philo);
	fork_locker(philo, left, right);
	print_message(EAT, philo);
	meal_timestamp(philo);
	ft_sleep(EAT, philo);
	pthread_mutex_unlock(&philo->shared->fork_mutex[right]);
	pthread_mutex_unlock(&philo->shared->fork_mutex[left]);
	pthread_mutex_lock(&philo->shared->meal);
	philo->shared->meals[philo->philo_id]++;
	pthread_mutex_unlock(&philo->shared->meal);
	print_message(SLEEP, philo);
	ft_sleep(SLEEP, philo);
	pthread_mutex_lock(&philo->shared->locker);
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
		simulation(philo, left, right);
	pthread_mutex_unlock(&philo->shared->locker);
	return (NULL);
}

int	philosophers(t_shared *shared)
{
	int	i;

	i = -1;
	pthread_mutex_init(&shared->locker, NULL);
	pthread_mutex_init(&shared->meal, NULL);
	while (++i < shared->input->num_of_philo)
		pthread_mutex_init(&shared->fork_mutex[i], NULL);
	pthread_create(&shared->monitor, NULL, monitor, (void *)shared);
	i = -1;
	while (++i < shared->input->num_of_philo)
	{
		pthread_create(&shared->philo[i]->philo_pth, NULL,
			philo_routine, (void *)shared->philo[i]);
	}
	i = -1;
	while (++i < shared->input->num_of_philo)
		pthread_join(shared->philo[i]->philo_pth, NULL);
	pthread_join(shared->monitor, NULL);
	pthread_mutex_destroy(&shared->locker);
	pthread_mutex_destroy(&shared->meal);
	return (0);
}
