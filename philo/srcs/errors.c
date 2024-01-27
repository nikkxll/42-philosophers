/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:39:31 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 23:24:35 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

int	struct_free(t_shared *shared, char *message, int status)
{
	t_philo **philo;
	
    philo = shared->philo;    
	if (shared->meals)
        free(shared->meals);    
	if (shared->last_meal_ts)
        free(shared->last_meal_ts);   
	if (shared->fork_mutex)
        free(shared->fork_mutex); 
	while (*philo)
    {       
		free(*philo);
        philo++;    
	}
	if (shared->philo)
		free(shared->philo);
	if (shared->input)
		free(shared->input);
	free(shared);
	if (status == 1)
		return (error_msg(message));
	else if (status == 2)
		return (2);
	else 
		return (0);
}
