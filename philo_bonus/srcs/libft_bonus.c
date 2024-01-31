/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:28:32 by dnikifor          #+#    #+#             */
/*   Updated: 2024/01/31 16:33:13 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers_bonus.h"

static long	numb_check(const char *str, int i, int j, long num)
{
	if (str[i] == '+')
	{
		i++;
		j++;
		while (str[j] != '\0')
		{
			if (str[j] == '+')
				return (-1);
			j++;
		}
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num += str[i++] - '0';
		if (num > INT_MAX)
			return (-1);
	}
	return (num);
}

long	ft_atol(const char *str)
{
	long	res;
	int		i;

	i = 0;
	while ((str[i] == '\t') || (str[i] == '\n') || (str[i] == '\v')
		|| (str[i] == '\f') || (str[i] == '\r') || (str[i] == ' '))
		i++;
	res = numb_check(str, i, i, 0);
	return (res);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*s++)
		i++;
	return (i);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*mem;
	size_t	num;

	num = (size_t)-1;
	if (count != 0 && (num / count) < size)
		return (NULL);
	num = count * size;
	mem = malloc(num);
	if (!mem)
		return (NULL);
	memset(mem, 0, num);
	return (mem);
}
