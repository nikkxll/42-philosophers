/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:14:09 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 16:33:08 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

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
	struct_free(shared, NULL, 0);
	return (0);
}
