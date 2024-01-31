/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 23:03:21 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 17:18:36 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	mutex_error(int status, t_code code)
{
	if (status != 0 && code == LOCK)
		return (error_msg("Error occured with pthread_mutex_lock.\n"));
	else if (status != 0 && code == UNLOCK)
		return (error_msg("Error occured with pthread_mutex_unlock.\n"));
	else if (status != 0 && code == INIT)
		return (error_msg("Error occured with pthread_mutex_init.\n"));
	else if (status != 0 && code == DESTROY)
		return (error_msg("Error occured with pthread_mutex_destroy.\n"));
	else
		return (0);
}

static int	thread_error(int status, t_code code)
{
	if (status != 0 && code == CREATE)
		return (error_msg("Error occured with pthread_create.\n"));
	else if (status != 0 && code == JOIN)
		return (error_msg("Error occured with pthread_join.\n"));
	else
		return (0);
}

int	mutex_wrapper(pthread_mutex_t *mutex, t_code code)
{
	if (code == LOCK)
		return (mutex_error(pthread_mutex_lock(mutex), code));
	else if (code == UNLOCK)
		return (mutex_error(pthread_mutex_unlock(mutex), code));
	else if (code == INIT)
		return (mutex_error(pthread_mutex_init(mutex, NULL), code));
	else if (code == DESTROY)
		return (mutex_error(pthread_mutex_destroy(mutex), code));
	else
		return (0);
}

int	thread_wrapper(pthread_t *thread, void *(*foo)(void *),
		void *data, t_code code)
{
	if (code == CREATE)
		return (thread_error(pthread_create(thread, NULL, foo, data),
				code));
	else if (code == JOIN)
		return (thread_error(pthread_join(*thread, NULL), code));
	else
		return (0);
}
