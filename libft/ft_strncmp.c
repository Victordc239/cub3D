/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 13:24:29 by sofernan          #+#    #+#             */
/*   Updated: 2025/11/17 13:45:38 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char const *s1, char const *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while (i < n && *s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
		i++;
	}
	if (i < n)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	else
		return (0);
}
