#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NO_OF_PHILOSOPHERS 200
#define TIME_TO_DIE 50

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
            long long current_timestamp = getTimestamp();
			// printf("%lld %d check\n", current_timestamp, j + 1);
			pthread_mutex_lock(&mutex_forks);
            if (current_timestamp - last_meal_timestamp[j] > TIME_TO_DIE && last_meal_timestamp[j])
			{
				printf("%lld %d died\n", current_timestamp, j + 1);
				printf("%lld %d died\n", current_timestamp - last_meal_timestamp[j], j + 1);
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
			{
				printf("%lld %d is thinking\n", getTimestamp(), i + 1);
				pthread_mutex_unlock(&mutex_forks);
				usleep(1000);
				pthread_mutex_lock(&mutex_forks);
			}
            waiting[i] = 1;
            pthread_mutex_unlock(&mutex_forks);
            while (forks[right] || forks[left])
                continue ;
            pthread_mutex_lock(&mutex_forks);
        }

        if (!forks[right] && !forks[left] && waiting[i] == 0)
		{
			printf("%lld %d is thinking\n", getTimestamp(), i + 1);
			pthread_mutex_unlock(&mutex_forks);
			usleep(1000);
			pthread_mutex_lock(&mutex_forks);
		}
        waiting[i] = 0;
        forks[right] = 1;
        forks[left] = 1;
        printf("%lld %d has taken a fork\n", getTimestamp(), i + 1);
        printf("%lld %d is eating\n", getTimestamp(), i + 1);
		last_meal_timestamp[i] = getTimestamp();
		pthread_mutex_unlock(&mutex_forks);

        usleep(300000);
		// printf("%lld %d put down fork %d\n", getTimestamp(), i + 1, left);
		// printf("%lld %d put down fork %d\n", getTimestamp(), i + 1, right);

        pthread_mutex_lock(&mutex_forks);
        forks[right] = 0;
        forks[left] = 0;	

        // for (int j = 0; j < NO_OF_PHILOSOPHERS; j++)
        // {
        //     if (waiting[j])	
        //     {
        //         pthread_mutex_unlock(&mutex_forks);
        //         usleep(100);
        //         pthread_mutex_lock(&mutex_forks);
        //         break;
        //     }
        // }

        pthread_mutex_unlock(&mutex_forks);

        printf("%lld %d is sleeping\n", getTimestamp(), i + 1);
        usleep(200000);
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
