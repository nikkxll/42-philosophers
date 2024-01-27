/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:14:09 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/27 16:30:49 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int argc, char **argv)
{
	t_shared	*shared;

	shared = (t_shared *)ft_calloc(1, sizeof(t_shared));
	if (!shared)
		return (error_msg("Malloc error\n"));
	if (initialization(argc, argv, shared))
		return (1);
	if (philosophers(shared))
		return (1);
	return (0);
}
