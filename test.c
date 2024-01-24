#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// cc -fsanitize=thread test.c

#define NO_OF_PHILOSOPHERS 199
#define TIME_TO_DIE 401
#define TIME_TO_EAT 100
#define TIME_TO_SLEEP 100
#define EAT_NUMBER 1

#define RESET_COLOR "\x1b[0m"
#define RED_COLOR "\x1b[31m"
#define GREEN_COLOR "\x1b[32m"
#define YELLOW_COLOR "\x1b[33m"
#define BLUE_COLOR "\x1b[34m"

pthread_t philosophers[NO_OF_PHILOSOPHERS];
pthread_t death_checker_thread;
pthread_mutex_t fork_mutex[NO_OF_PHILOSOPHERS];
pthread_mutex_t mutex_locker = PTHREAD_MUTEX_INITIALIZER;
struct timeval tv;
int forks[NO_OF_PHILOSOPHERS];
int meals[NO_OF_PHILOSOPHERS];
int waiting[NO_OF_PHILOSOPHERS] = {0};
long long last_meal_timestamp[NO_OF_PHILOSOPHERS] = {0};
int locker = 1;

int meals_checker(int *arr, int size)
{
	int i = 0;

	while (i < NO_OF_PHILOSOPHERS)
	{
		if (arr[i] >= EAT_NUMBER)
		{
			i++;
			continue;
		}
		else
			return (0);
	}
	return (1);
}

void init()
{
	int i;
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
    {
        forks[i] = 0;
        pthread_mutex_init(&fork_mutex[i], NULL);
    }
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
		forks[i] = 0;
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
		meals[i] = 0;
}

long long getTimestamp()
{
	gettimeofday(&tv, NULL);
	return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void *death_monitor(void *arg)
{
	int j;
	while (1)
	{
		pthread_mutex_lock(&mutex_locker);
		if (EAT_NUMBER && meals_checker(meals, NO_OF_PHILOSOPHERS))
		{
			locker = 0;
			pthread_mutex_unlock(&mutex_locker);
			printf(RED_COLOR "%lld End of simulation\n" RESET_COLOR, getTimestamp());
			return (NULL);
		}
		for (j = 0; j < NO_OF_PHILOSOPHERS; j++)
		{
			long long current_timestamp = getTimestamp();
			if (current_timestamp - last_meal_timestamp[j] > TIME_TO_DIE && last_meal_timestamp[j])
			{
				locker = 0;
				pthread_mutex_unlock(&mutex_locker);
				printf(RED_COLOR "%lld %d died\n" RESET_COLOR, current_timestamp, j + 1);
				return (NULL);
			}
		}
		pthread_mutex_unlock(&mutex_locker);
	}
}

void *philosopher(void *arg)
{
	int i = *(int *)arg;
	int right = i;
	int left = (i + NO_OF_PHILOSOPHERS - 1) % NO_OF_PHILOSOPHERS;

	pthread_mutex_lock(&mutex_locker);
	while (locker)
	{
		pthread_mutex_unlock(&mutex_locker);

		pthread_mutex_lock(&mutex_locker);
		last_meal_timestamp[i] = getTimestamp();

		while (forks[right] || forks[left])
		{
			if (locker && waiting[i] == 0)
				printf(BLUE_COLOR "%lld %d is thinking\n" RESET_COLOR, getTimestamp(), i + 1);
			waiting[i] = 1;

			pthread_mutex_unlock(&mutex_locker);
			usleep(100);
			pthread_mutex_lock(&mutex_locker);
		}
		if (locker && !forks[right] && !forks[left] && waiting[i] == 0)
			printf(BLUE_COLOR "%lld %d is thinking\n" RESET_COLOR, getTimestamp(), i + 1);
		waiting[i] = 0;

		// pthread_mutex_lock(&fork_mutex[right]);
		forks[right] = 1;
		// pthread_mutex_unlock(&fork_mutex[right]);
		// pthread_mutex_lock(&fork_mutex[left]);
		forks[left] = 1;
		// pthread_mutex_unlock(&fork_mutex[left]);

		if (locker)
		{
			printf(YELLOW_COLOR "%lld %d has taken a fork\n" RESET_COLOR, getTimestamp(), i + 1);
			printf(YELLOW_COLOR "%lld %d is eating\n" RESET_COLOR, getTimestamp(), i + 1);
		}
		last_meal_timestamp[i] = getTimestamp();
		pthread_mutex_unlock(&mutex_locker);
		usleep(TIME_TO_EAT * 1000);
		pthread_mutex_lock(&mutex_locker);
		meals[i]++;

		// pthread_mutex_lock(&fork_mutex[right]);
		forks[right] = 0;
		// pthread_mutex_unlock(&fork_mutex[right]);
		// pthread_mutex_lock(&fork_mutex[left]);
		forks[left] = 0;
		// pthread_mutex_unlock(&fork_mutex[left]);

		if (locker)
			printf(GREEN_COLOR "%lld %d is sleeping\n" RESET_COLOR, getTimestamp(), i + 1);
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
	int i;
	int philosopher_ids[NO_OF_PHILOSOPHERS];

	pthread_mutex_init(&mutex_locker, NULL);

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
