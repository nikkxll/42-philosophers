/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:52:48 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/26 22:40:57 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	initialization(int ac, char **av, t_shared *shared)
{
	shared->input = (t_input *)ft_calloc(1, sizeof(t_input));
	if (!shared->input)
		return (error_struct_free(shared, "Malloc error\n"));
	if (reader(ac, av, shared))
		return (1);
	if (data_initialization(shared))
		return (1);
	return (0);
}
