/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 16:09:28 by dnikifor         ###   ########.fr       */
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
	long int	time;

	time = get_timestamp(philo);
	while (get_timestamp(philo) - time < ms)
		usleep(ms / 10);
}

void	*monitor(void *arg)
{
	int			j;
	long long	current_timestamp;
	t_shared	*shared;

	shared = (t_shared *)arg;
	while (1)
	{
		pthread_mutex_lock(&shared->meal);
		if (shared->eat_number
			&& meals_checker(shared->meals, shared->num_of_philo,
				shared->eat_number))
		{
			pthread_mutex_unlock(&shared->meal);
			pthread_mutex_lock(&shared->locker);
			shared->flag_locker = 0;
			pthread_mutex_unlock(&shared->locker);
			return (NULL);
		}
		pthread_mutex_unlock(&shared->meal);
		j = -1;
		while (++j < shared->num_of_philo)
		{
			pthread_mutex_lock(&shared->meal);
			current_timestamp = get_timestamp(shared->philo[j]);
			if (current_timestamp - shared->last_meal_ts[j] >=
				shared->time_to_die / 1000
				&& shared->last_meal_ts[j])
			{
				pthread_mutex_unlock(&shared->meal);
				pthread_mutex_lock(&shared->locker);
				shared->flag_locker = 0;
				pthread_mutex_unlock(&shared->locker);
				printf(RED_COLOR "%lld %d died\n" RESET_COLOR,
					current_timestamp, j + 1);
				return (NULL);
			}
			pthread_mutex_unlock(&shared->meal);
		}
	}
}

void	*philosopher(void *arg)
{
	t_philo	*philo;
	int		i;
	int		right;
	int		left;

	philo = (t_philo *)arg;
	i = philo->philo_id;
	right = i;
	left = (i + philo->shared->num_of_philo - 1) % philo->shared->num_of_philo;
	pthread_mutex_lock(&philo->shared->locker);
	while (philo->shared->flag_locker)
	{
		pthread_mutex_unlock(&philo->shared->locker);
		
		pthread_mutex_lock(&philo->shared->meal);
		philo->shared->last_meal_ts[i] = get_timestamp(philo);
		pthread_mutex_unlock(&philo->shared->meal);

		pthread_mutex_lock(&philo->shared->locker);
		if (philo->shared->flag_locker)
			printf("%lld %d is thinking\n", get_timestamp(philo), i + 1);
		pthread_mutex_unlock(&philo->shared->locker);
		
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
		
		pthread_mutex_lock(&philo->shared->locker);
		if (philo->shared->flag_locker)
		{
			printf("%lld %d has taken a fork\n", get_timestamp(philo), i + 1);
			printf("%lld %d has taken a fork\n", get_timestamp(philo), i + 1);
			printf("%lld %d is eating\n", get_timestamp(philo), i + 1);
		}
		pthread_mutex_unlock(&philo->shared->locker);
		
		pthread_mutex_lock(&philo->shared->meal);
		philo->shared->last_meal_ts[i] = get_timestamp(philo);
		pthread_mutex_unlock(&philo->shared->meal);
		
		ft_usleep(philo->shared->time_to_eat / 1000, philo);
		pthread_mutex_unlock(&philo->shared->fork_mutex[right]);
		pthread_mutex_unlock(&philo->shared->fork_mutex[left]);

		pthread_mutex_lock(&philo->shared->meal);
		philo->shared->meals[i]++;
		pthread_mutex_unlock(&philo->shared->meal);
		
		pthread_mutex_lock(&philo->shared->locker);
		if (philo->shared->flag_locker)
			printf("%lld %d is sleeping\n", get_timestamp(philo), i + 1);
		pthread_mutex_unlock(&philo->shared->locker);
		
		ft_usleep(philo->shared->time_to_sleep / 1000, philo);

		pthread_mutex_lock(&philo->shared->locker);
	}
	pthread_mutex_unlock(&philo->shared->locker);
	return (NULL);
}

int	philosophers(t_shared *shared)
{
	int	i;

	i = -1;
	pthread_mutex_init(&shared->locker, NULL);
	pthread_mutex_init(&shared->meal, NULL);

	while (++i < shared->num_of_philo)
		pthread_mutex_init(&shared->fork_mutex[i], NULL);

	pthread_create(&shared->monitor, NULL, monitor, (void *)shared);
	i = -1;
	while (++i < shared->num_of_philo)
	{
		pthread_create(&shared->philo[i]->philo_pth, NULL,
			philosopher, (void *)shared->philo[i]);
	}
	i = -1;
	while (++i < shared->num_of_philo)
		pthread_join(shared->philo[i]->philo_pth, NULL);

	pthread_join(shared->monitor, NULL);

	pthread_mutex_destroy(&shared->locker);
	pthread_mutex_destroy(&shared->meal);

	return (0);
}
