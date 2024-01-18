/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 23:03:21 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/18 23:40:21 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	mutex_error(int status, t_code code)
{

}

static int	thread_error(int status, t_code code)
{

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
		return (1);
}

int	thread_wrapper(pthread_t *thread, void *(*foo)(void *),
		void *data, t_code code)
{
	if (code == CREATE)
		return (handle_thread_error(pthread_create(thread, NULL, foo, data),
			code));
	else if (code == JOIN)
		return (handle_thread_error(pthread_join(*thread, NULL), code));
	else if (code == DETACH)
		return (handle_thread_error(pthread_detach(*thread), code));
	else
		return (1);
}