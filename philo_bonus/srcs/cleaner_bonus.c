/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:39:31 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/01 00:05:49 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

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
