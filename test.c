/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:19:30 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/25 15:37:34 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define NO_OF_PHILOSOPHERS 4
#define TIME_TO_DIE 205
#define TIME_TO_EAT 200
#define TIME_TO_SLEEP 2
#define EAT_NUMBER 1

#define RESET_COLOR "\x1b[0m"
#define RED_COLOR "\x1b[31m"

pthread_t philosophers[NO_OF_PHILOSOPHERS];
pthread_mutex_t fork_mutex[NO_OF_PHILOSOPHERS];
pthread_t death_checker_thread;
pthread_mutex_t mutex_locker;
struct timeval tv;
int meals[NO_OF_PHILOSOPHERS];
long long last_meal_timestamp[NO_OF_PHILOSOPHERS];
int locker = 1;

int	meals_checker(int *arr, int size)
{
	int	i;

	i = 0;
	while (i < NO_OF_PHILOSOPHERS)
	{
		if (arr[i] >= EAT_NUMBER)
		{
			i++;
			continue ;
		}
		else
			return (0);
	}
	return (1);
}

void	init()
{
	memset(meals, 0, NO_OF_PHILOSOPHERS * sizeof(int));
	memset(last_meal_timestamp, 0, NO_OF_PHILOSOPHERS * sizeof(long long));
}

long long	getTimestamp()
{
	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

void	*death_monitor(void *arg)
{
	int			j;
	long long	current_timestamp;

	while (1)
	{
		pthread_mutex_lock(&mutex_locker);
		if (EAT_NUMBER && meals_checker(meals, NO_OF_PHILOSOPHERS))
		{
			locker = 0;
			pthread_mutex_unlock(&mutex_locker);
			printf(RED_COLOR "End of simulation\n" RESET_COLOR);
			return (NULL);
		}
		j = 0;
		while (j < NO_OF_PHILOSOPHERS)
		{
			current_timestamp = getTimestamp();
			if (current_timestamp - last_meal_timestamp[j] > TIME_TO_DIE
				&& last_meal_timestamp[j])
			{
				locker = 0;
				pthread_mutex_unlock(&mutex_locker);
				printf(RED_COLOR "%lld %d died\n" RESET_COLOR,
					current_timestamp, j + 1);
				return (NULL);
			}
			j++;
		}
		pthread_mutex_unlock(&mutex_locker);
	}
}

void	*philosopher(void *arg)
{
	int	i;
	int	right;
	int	left;

	i = *(int *)arg;
	right = i;
	left = (i + NO_OF_PHILOSOPHERS - 1) % NO_OF_PHILOSOPHERS;

	pthread_mutex_lock(&mutex_locker);
	while (locker)
	{
		pthread_mutex_unlock(&mutex_locker);

		pthread_mutex_lock(&mutex_locker);
		last_meal_timestamp[i] = getTimestamp();
		pthread_mutex_unlock(&mutex_locker);

		pthread_mutex_lock(&mutex_locker);
		if (locker)
			printf("%lld %d is thinking\n", getTimestamp(), i + 1);
		pthread_mutex_unlock(&mutex_locker);

		if (right < left)
		{
			pthread_mutex_lock(&fork_mutex[right]);
			pthread_mutex_lock(&fork_mutex[left]);
		}
		else
		{
			pthread_mutex_lock(&fork_mutex[left]);
			pthread_mutex_lock(&fork_mutex[right]);
		}

		pthread_mutex_lock(&mutex_locker);
		if (locker)
		{
			printf("%lld %d has taken a fork\n", getTimestamp(), i + 1);
			printf("%lld %d has taken a fork\n", getTimestamp(), i + 1);
			printf("%lld %d is eating\n", getTimestamp(), i + 1);
		}
		last_meal_timestamp[i] = getTimestamp();
		pthread_mutex_unlock(&mutex_locker);

		usleep(TIME_TO_EAT * 1000);
		pthread_mutex_unlock(&fork_mutex[right]);
		pthread_mutex_unlock(&fork_mutex[left]);

		pthread_mutex_lock(&mutex_locker);
		meals[i]++;
		if (locker)
			printf("%lld %d is sleeping\n", getTimestamp(), i + 1);
		pthread_mutex_unlock(&mutex_locker);

		usleep(TIME_TO_SLEEP * 1000);

		pthread_mutex_lock(&mutex_locker);
	}
	pthread_mutex_unlock(&mutex_locker);
	return (NULL);
}

int main()
{
	init();
	int	i;
	int	philosopher_ids[NO_OF_PHILOSOPHERS];

	pthread_mutex_init(&mutex_locker, NULL);

	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
		pthread_mutex_init(&fork_mutex[i], NULL);

	pthread_create(&death_checker_thread, NULL, death_monitor, NULL);

	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
	{
		philosopher_ids[i] = i;
		pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopher_ids[i]);
	}

	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
		pthread_join(philosophers[i], NULL);

	pthread_join(death_checker_thread, NULL);

	pthread_mutex_destroy(&mutex_locker);

	return 0;
} 
