/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:39:31 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 11:52:00 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

void	kill_process(t_shared *shared, int i, int j)
{
	while (++i < j)
	{
		if (kill(shared->pid[i], SIGINT) == 1)
		{
			error_msg("kill error\n");
			exit(EXIT_FAILURE);
		}
	}
}

void	semaphore_closing(t_shared *shared)
{
	if (shared->forks)
	{
		sem_wrapper(shared->forks, CLOSE);
		sem_unlink("/forks");
	}
	if (shared->locker)
	{
		sem_wrapper(shared->locker, CLOSE);
		sem_unlink("/locker");
	}
	if (shared->end)
	{
		sem_wrapper(shared->end, CLOSE);
		sem_unlink("/end");
	}
	if (shared->death)
	{
		sem_wrapper(shared->death, CLOSE);
		sem_unlink("/death");
	}
}

int	semaphore_clearing(t_shared *shared, int flag, int i, int j)
{
	if (flag)
		kill_process(shared, i, j);
	if (sem_wrapper(shared->death, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	if (sem_wrapper(shared->end, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	if (sem_wrapper(shared->forks, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	if (sem_wrapper(shared->locker, CLOSE))
		return (struct_free(shared, "Semaphore close error\n", 1));
	sem_unlink("/forks");
	sem_unlink("/locker");
	sem_unlink("/end");
	sem_unlink("/death");
	return (0);
}

int	error_msg(char *msg)
{
	write(2, RED_COLOR, sizeof(RED_COLOR));
	write(2, msg, ft_strlen(msg));
	write(2, RESET_COLOR, sizeof(RESET_COLOR));
	return (1);
}

int	struct_free(t_shared *shared, char *message, int status)
{
	if (shared->pid)
		free(shared->pid);
	if (shared->input)
		free(shared->input);
	free(shared);
	if (status == 1)
		return (error_msg(message));
	else if (status == 2)
		return (1);
	return (0);
}
