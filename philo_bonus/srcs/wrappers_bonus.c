/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 23:03:21 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 00:34:34 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

void	wait_post_wrapper(t_shared *shared, sem_t *semaphore, int procedure)
{
	if (procedure == WAIT)
	{
		if (sem_wrapper(semaphore, WAIT))
			semaphore_parent_control(shared);
	}
	else
	{
		if (sem_wrapper(semaphore, POST))
			semaphore_parent_control(shared);
	}
}

static int	semaphore_error(int status, t_code code)
{
	if (status != 0 && code == WAIT)
		return (error_msg("Error occured with sem_wait.\n"));
	else if (status != 0 && code == POST)
		return (error_msg("Error occured with sem_post.\n"));
	else if (status != 0 && code == CLOSE)
		return (error_msg("Error occured with sem_close.\n"));
	else
		return (0);
}

static int	thread_error(int status, t_code code)
{
	if (status != 0 && code == CREATE)
		return (error_msg("Error occured with pthread_create.\n"));
	else if (status != 0 && code == DETACH)
		return (error_msg("Error occured with pthread_detach.\n"));
	else
		return (0);
}

int	sem_wrapper(sem_t *semaphore, t_code code)
{
	if (code == WAIT)
		return (semaphore_error(sem_wait(semaphore), code));
	else if (code == POST)
		return (semaphore_error(sem_post(semaphore), code));
	else if (code == CLOSE)
		return (semaphore_error(sem_close(semaphore), code));
	else
		return (0);
}

int	thread_wrapper(pthread_t *thread, void *(*foo)(void *),
		void *data, t_code code)
{
	if (code == CREATE)
		return (thread_error(pthread_create(thread, NULL, foo, data),
				code));
	else if (code == DETACH)
		return (thread_error(pthread_detach(*thread), code));
	else
		return (0);
}
