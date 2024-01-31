/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 23:10:49 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 19:14:24 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_BONUS_H
# define CONSTANTS_BONUS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <limits.h>
# include <string.h>

# ifndef RED_COLOR
#  define RED_COLOR "\x1b[31m"
# endif

# ifndef RESET_COLOR
#  define RESET_COLOR "\x1b[0m"
# endif

# ifndef THINK
#  define THINK 0
# endif

# ifndef EAT
#  define EAT 1
# endif

# ifndef SLEEP
#  define SLEEP 2
# endif

typedef enum e_code
{
	WAIT,
	POST,
	CLOSE,
	CREATE,
	DETACH,
}	t_code;

#endif