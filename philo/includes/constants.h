/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 23:10:49 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 23:12:03 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

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
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_code;

#endif