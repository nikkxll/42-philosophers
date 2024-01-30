#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <signal.h>

#define NO_OF_PHILOSOPHERS 4
#define TIME_TO_DIE 201
#define TIME_TO_EAT 100
#define TIME_TO_SLEEP 100
#define NUM_MEALS 5

#define RESET_COLOR "\x1b[0m"
#define RED_COLOR "\x1b[31m"
#define GREEN_COLOR "\x1b[32m"
#define YELLOW_COLOR "\x1b[33m"
#define BLUE_COLOR "\x1b[34m"

pthread_t philosophers[NO_OF_PHILOSOPHERS];
pthread_t death_checker_thread;
sem_t *forks;
sem_t *locker;
sem_t *end;
sem_t *death;
long long last_meal_timestamp;
int number_of_meals = 0;
pid_t pid[NO_OF_PHILOSOPHERS];

long long getTimestamp()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void *death_monitor(void *arg)
{
	int j;
	long long current_timestamp;

	current_timestamp = getTimestamp();
	while (1)
	{
		sem_wait(death);
		// printf("%lld\n", current_timestamp - last_meal_timestamp);
		current_timestamp = getTimestamp();
		if (current_timestamp - last_meal_timestamp >= TIME_TO_DIE && last_meal_timestamp)
		{
			sem_wait(locker);
			printf(RED_COLOR "%lld %d died\n" RESET_COLOR, current_timestamp, j + 1);
			for (int i = 0; i < NO_OF_PHILOSOPHERS; ++i)
				sem_post(end);
			return (NULL);
		}
		sem_post(death);
	}
}

void routine(int i)
{
	pthread_create(&death_checker_thread, NULL, death_monitor, NULL);
	pthread_detach(death_checker_thread);

	last_meal_timestamp = getTimestamp();
	while (1)
	{
		sem_wait(locker);
		printf(BLUE_COLOR "%lld %d is thinking\n" RESET_COLOR, getTimestamp(), i + 1);
		sem_post(locker);

		sem_wait(forks);
		sem_wait(forks);
		sem_wait(locker);
		printf(YELLOW_COLOR "%lld %d has taken a fork\n" RESET_COLOR, getTimestamp(), i + 1);
		printf(YELLOW_COLOR "%lld %d is eating\n" RESET_COLOR, getTimestamp(), i + 1);
		sem_post(locker);
		last_meal_timestamp = getTimestamp();
		usleep(TIME_TO_EAT * 1000);

		sem_wait(locker);
		number_of_meals++;
		if (number_of_meals == NUM_MEALS)
			sem_post(end);
		sem_post(locker);

		sem_post(forks);
		sem_post(forks);

		sem_wait(locker);
		printf(GREEN_COLOR "%lld %d is sleeping\n" RESET_COLOR, getTimestamp(), i + 1);
		sem_post(locker);

		usleep(TIME_TO_SLEEP * 1000);
	}
}

int main()
{
	int i;
	int philosopher_ids[NO_OF_PHILOSOPHERS];

	sem_unlink("/forks");
	sem_unlink("/locker");
	sem_unlink("/end");
	sem_unlink("/death");
	forks = sem_open("/forks", O_CREAT, 0644, NO_OF_PHILOSOPHERS);
	locker = sem_open("/locker", O_CREAT, 0644, 1);
	end = sem_open("/end", O_CREAT, 0644, NO_OF_PHILOSOPHERS);
	death = sem_open("/death", O_CREAT, 0644, 1);

	for (int i = 0; i < NO_OF_PHILOSOPHERS; ++i)
		sem_wait(end);
	for (int i = 0; i < NO_OF_PHILOSOPHERS; ++i)
	{
		philosopher_ids[i] = i;
		pid[i] = fork();

		if (pid[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid[i] == 0)
		{
			routine(philosopher_ids[i]);
			exit(EXIT_SUCCESS);
		}
	}
	for (int i = 0; i < NO_OF_PHILOSOPHERS; ++i)
		sem_wait(end);

	// for (int i = 0; i < NO_OF_PHILOSOPHERS; ++i)
	// 	waitpid(pid[i++], 0, 0);
	for (int i = 0; i < NO_OF_PHILOSOPHERS; ++i)
		kill(pid[i], SIGINT);
	sem_close(death);
	sem_close(end);
	sem_close(forks);
	sem_close(locker);
	sem_unlink("/forks");
	sem_unlink("/locker");
	sem_unlink("/end");
	sem_unlink("/death");
	return 0;
}
