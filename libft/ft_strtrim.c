/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:21:27 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/03/24 11:21:27 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_find_set(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	find;

	i = 0;
	while (s1[i] != '\0')
	{
		j = 0;
		find = 0;
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				find = 1;
			j++;
		}
		if (find == 0)
			return (i);
		i++;
	}
	return (i);
}

static int	ft_find_set_final(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	find;

	i = ft_strlen(s1) - 1;
	while (i >= 0)
	{
		j = 0;
		find = 0;
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				find = 1;
			j++;
		}
		if (find == 0)
			return (i);
		i--;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	k;
	char	*result;

	i = ft_find_set(s1, set);
	j = ft_find_set_final(s1, set);
	k = 0;
	if (i >= ft_strlen(s1))
	{
		result = (char *)malloc(1);
		if (!result)
			return (NULL);
		result[0] = '\0';
		return (result);
	}
	result = (char *)malloc(sizeof(char) * (j - i + 2));
	if (!result)
		return (NULL);
	while (i <= j)
	{
		result[k++] = s1[i++];
	}
	result[k] = '\0';
	return (result);
}
/*
int	main(void)
{
	char const	s1[] = "---HO--LA---";
	char const	set[] = "-";
	printf("Resultado: %s\n", ft_strtrim(s1, set));
	return(0);
}*/