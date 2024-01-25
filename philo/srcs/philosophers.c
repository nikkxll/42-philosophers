/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:58:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/25 19:18:42 by dnikifor         ###   ########.fr       */
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
	gettimeofday(&philo->tv, NULL);
	return ((long long)philo->tv.tv_sec * 1000
		+ (long long)philo->tv.tv_usec / 1000);
}

void	*monitor(void *arg)
{
	int			j;
	long long	current_timestamp;
	t_philo		*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->locker);
		if (philo->input->eat_number
			&& meals_checker(philo->meals, philo->input->num_of_philo,
				philo->input->eat_number))
		{
			philo->flag_locker = 0;
			pthread_mutex_unlock(&philo->locker);
			printf(RED_COLOR "End of simulation\n" RESET_COLOR);
			return (NULL);
		}
		j = -1;
		while (++j < philo->input->num_of_philo)
		{
			current_timestamp = get_timestamp(philo);
			if (current_timestamp - philo->last_meal_ts[j] >
				philo->input->time_to_die
				&& philo->last_meal_ts[j])
			{
				philo->flag_locker = 0;
				pthread_mutex_unlock(&philo->locker);
				printf(RED_COLOR "%lld %d died\n" RESET_COLOR,
					current_timestamp, j + 1);
				return (NULL);
			}
		}
		pthread_mutex_unlock(&philo->locker);
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
	left = (i + philo->input->num_of_philo - 1) % philo->input->num_of_philo;

	pthread_mutex_lock(&philo->locker);
	while (philo->flag_locker)
	{
		pthread_mutex_unlock(&philo->locker);

		pthread_mutex_lock(&philo->locker);
		philo->last_meal_ts[i] = get_timestamp(philo);
		pthread_mutex_unlock(&philo->locker);

		pthread_mutex_lock(&philo->locker);
		if (philo->flag_locker)
			printf("%lld %d is thinking\n", get_timestamp(philo), i + 1);
		pthread_mutex_unlock(&philo->locker);

		if (right < left)
		{
			pthread_mutex_lock(&philo->fork_mutex[right]);
			pthread_mutex_lock(&philo->fork_mutex[left]);
		}
		else
		{
			pthread_mutex_lock(&philo->fork_mutex[left]);
			pthread_mutex_lock(&philo->fork_mutex[right]);
		}

		pthread_mutex_lock(&philo->locker);
		if (philo->flag_locker)
		{
			printf("%lld %d has taken a fork\n", get_timestamp(philo), i + 1);
			printf("%lld %d has taken a fork\n", get_timestamp(philo), i + 1);
			printf("%lld %d is eating\n", get_timestamp(philo), i + 1);
		}
		philo->last_meal_ts[i] = get_timestamp(philo);
		pthread_mutex_unlock(&philo->locker);

		usleep(philo->input->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->fork_mutex[right]);
		pthread_mutex_unlock(&philo->fork_mutex[left]);

		pthread_mutex_lock(&philo->locker);
		philo->meals[i]++;
		if (philo->flag_locker)
			printf("%lld %d is sleeping\n", get_timestamp(philo), i + 1);
		pthread_mutex_unlock(&philo->locker);

		usleep(philo->input->time_to_sleep * 1000);

		pthread_mutex_lock(&philo->locker);
	}
	pthread_mutex_unlock(&philo->locker);
	return (NULL);
}

int	philosophers(t_philo *philo)
{
	int	i;

	i = -1;
	pthread_mutex_init(&philo->locker, NULL);

	while (++i < philo->input->num_of_philo)
		pthread_mutex_init(&philo->fork_mutex[i], NULL);

	pthread_create(&philo->monitor, NULL, monitor, (void *)philo);
	i = -1;
	while (++i < philo->input->num_of_philo)
	{
		philo->philo_id = i;
		pthread_create(&philo->philosophers[i], NULL,
			philosopher, (void *)philo);
	}
	i = -1;
	while (++i < philo->input->num_of_philo)
		pthread_join(philo->philosophers[i], NULL);

	pthread_join(philo->monitor, NULL);

	pthread_mutex_destroy(&philo->locker);

	return (0);
}
