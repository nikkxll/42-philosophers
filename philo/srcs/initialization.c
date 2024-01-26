/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:52:48 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 09:45:11 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	initialization(int ac, char **av, t_shared *shared)
{
	if (reader(ac, av, shared))
		return (1);
	if (data_initialization(shared))
		return (1);
	return (0);
}
