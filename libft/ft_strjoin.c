/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:05:31 by sofernan          #+#    #+#             */
/*   Updated: 2025/11/17 13:45:20 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char const *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;

	s1_len = 0;
	s2_len = 0;
	i = -1;
	j = -1;
	if (!s1 || !s2)
		return (NULL);
	while (s1[s1_len])
		s1_len++;
	while (s2[s2_len])
		s2_len++;
	result = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	result[i + j] = '\0';
	return (result);
}
