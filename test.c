#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NO_OF_PHILOSOPHERS 3
#define TIME_TO_DIE 105
#define TIME_TO_EAT 100
#define TIME_TO_SLEEP 100

#define RESET_COLOR "\x1b[0m"
#define RED_COLOR "\x1b[31m"
#define GREEN_COLOR "\x1b[32m"
#define YELLOW_COLOR "\x1b[33m"
#define BLUE_COLOR "\x1b[34m"

pthread_t philosophers[NO_OF_PHILOSOPHERS];
pthread_t death_checker_thread;
pthread_mutex_t mutex_forks = PTHREAD_MUTEX_INITIALIZER;
int forks[NO_OF_PHILOSOPHERS];
int waiting[NO_OF_PHILOSOPHERS] = {0};
long long last_meal_timestamp[NO_OF_PHILOSOPHERS] = {0};

void init()
{
    int i;
    for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
        forks[i] = 0;
}

long long getTimestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void *death_checker(void *arg)
{
    int j;
    while (1)
    {
        for (j = 0; j < NO_OF_PHILOSOPHERS; j++)
        {
			pthread_mutex_lock(&mutex_forks);
            long long current_timestamp = getTimestamp();
            if (current_timestamp - last_meal_timestamp[j] > TIME_TO_DIE && last_meal_timestamp[j])
			{
				printf(RED_COLOR "%lld %d died\n" RESET_COLOR, current_timestamp, j + 1);
				// printf("%lld %d died\n", current_timestamp - last_meal_timestamp[j], j + 1);
				exit (1);
			}
			pthread_mutex_unlock(&mutex_forks);
        }
        
    }
}

void *philosopher(void *arg)
{
    int i = *(int *)arg;
    int right = i;
    int left = (i + NO_OF_PHILOSOPHERS - 1) % NO_OF_PHILOSOPHERS;

    while (1)
    {
        pthread_mutex_lock(&mutex_forks);
		last_meal_timestamp[i] = getTimestamp();
        pthread_mutex_unlock(&mutex_forks);

        pthread_mutex_lock(&mutex_forks);

        while (forks[right] || forks[left])
        {
            if (waiting[i] == 0)
				printf(BLUE_COLOR "%lld %d is thinking\n" RESET_COLOR, getTimestamp(), i + 1);
            waiting[i] = 1;

            pthread_mutex_unlock(&mutex_forks);
            usleep(1000);
            pthread_mutex_lock(&mutex_forks);
        }

        if (!forks[right] && !forks[left] && waiting[i] == 0)
			printf(BLUE_COLOR "%lld %d is thinking\n" RESET_COLOR, getTimestamp(), i + 1);
        waiting[i] = 0;
        forks[right] = 1;
        forks[left] = 1;

		pthread_mutex_unlock(&mutex_forks);
        printf(GREEN_COLOR "%lld %d has taken a fork\n" RESET_COLOR, getTimestamp(), i + 1);
        printf(GREEN_COLOR "%lld %d is eating\n" RESET_COLOR, getTimestamp(), i + 1);
        pthread_mutex_lock(&mutex_forks);
		last_meal_timestamp[i] = getTimestamp();
        pthread_mutex_unlock(&mutex_forks);

        usleep(TIME_TO_EAT * 1000);
		// printf(GREEN_COLOR "%lld %d put down fork %d\n" RESET_COLOR, getTimestamp(), i + 1, left);
		// printf(GREEN_COLOR "%lld %d put down fork %d\n" RESET_COLOR, getTimestamp(), i + 1, right);

        pthread_mutex_lock(&mutex_forks);

        forks[right] = 0;
        forks[left] = 0;	

        pthread_mutex_unlock(&mutex_forks);

        printf(YELLOW_COLOR "%lld %d is sleeping\n" RESET_COLOR, getTimestamp(), i + 1);
        usleep(TIME_TO_SLEEP * 1000);
    }
}

int main()
{
    init();
    int i;
    int philosopher_ids[NO_OF_PHILOSOPHERS];

	pthread_create(&death_checker_thread, NULL, death_checker, NULL);

    for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
    {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopher_ids[i]);
    }

    for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
        pthread_join(philosophers[i], NULL);
	
	pthread_join(death_checker_thread, NULL);

    return 0;
}