#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define N 5
int		array[N];
sem_t	*semaphores;
struct timeval tv;

void *thread_function(void *arg)
{
	int id = *(int *)arg;
	while (1)
	{
		sem_wait(semaphores);
		sem_wait(semaphores);
		gettimeofday(&tv, NULL);
		long x = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		printf("%ld Philosopher %d is eating.\n", tv.tv_sec * 1000 + tv.tv_usec / 1000, id + 1);
		while (tv.tv_sec * 1000 + tv.tv_usec / 1000 < x + 100)
		{
			gettimeofday(&tv, NULL);
			usleep(10);
		}
		sem_post(semaphores);
		sem_post(semaphores);
	}
	printf("%d\n", id);
	return NULL;
}

void init()
{
	for (int i = 0; i < N; ++i)
		array[i] = i;
}

int main()
{
	pthread_t	tid;
	pid_t		terminated_pid;
	int			status;

	init();
	sem_unlink("/forks_block");
	semaphores = sem_open("/forks_block", O_CREAT | O_EXCL, 0644, N);
	for (int i = 0; i < N; ++i)
	{
		pid_t pid = fork();

		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			pthread_create(&tid, NULL, thread_function, (void *)&array[i]);
			pthread_join(tid, NULL);
			exit(EXIT_SUCCESS);
		}
	}

	for (int i = 0; i < N; ++i)
	{
		terminated_pid = waitpid(-1, &status, 0);

		if (terminated_pid == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
	sem_unlink("/forks_block");
	return 0;
}
