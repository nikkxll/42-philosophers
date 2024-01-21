#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_OF_PHILOSOPHERS 6

pthread_t philosophers[NO_OF_PHILOSOPHERS];
pthread_mutex_t mutex_forks = PTHREAD_MUTEX_INITIALIZER;
int forks[NO_OF_PHILOSOPHERS];
int waiting[NO_OF_PHILOSOPHERS] = {0};

void init()
{
    int i;
    for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
        forks[i] = 0;
}

void *philosopher(void *arg)
{
    int i = *(int *)arg;
    int right = i;
    int left = (i + NO_OF_PHILOSOPHERS - 1) % NO_OF_PHILOSOPHERS;

    while (1)
    {
        pthread_mutex_lock(&mutex_forks);

        while (forks[right] || forks[left])
        {
            if (waiting[i] == 0)
                printf("Philosopher %d is thinking\n", i);
            waiting[i] = 1;
            pthread_mutex_unlock(&mutex_forks);
            while (forks[right] || forks[left])
                continue ;
            pthread_mutex_lock(&mutex_forks);
        }

        if (!forks[right] && !forks[left] && waiting[i] == 0)
            printf("Philosopher %d is thinking\n", i);

        waiting[i] = 0;
        forks[right] = 1;
        printf("Philosopher %d has taken right fork\n", i);
        forks[left] = 1;
        printf("Philosopher %d has taken left fork\n", i);

        pthread_mutex_unlock(&mutex_forks);

        printf("Philosopher %d is eating\n", i);
        usleep(500000);
        printf("Philosopher %d done with eating\n", i);

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

        printf("Philosopher %d is sleeping\n", i);
        usleep(500000);
    }
}

int main()
{
    init();
    int i;
    int philosopher_ids[NO_OF_PHILOSOPHERS];

    for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
    {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopher_ids[i]);
    }

    for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
        pthread_join(philosophers[i], NULL);

    return 0;
}
