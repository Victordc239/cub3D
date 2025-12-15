/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 15:59:34 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/12/15 16:03:15 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_texture_path(const char *s)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == '/' && *(s + 1) == '/')
			return (1);
		s++;
	}
	return (0);
}

int	check_texture_extension(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 4)
		return (0);
	return (ft_strcmp(s + len - 4, ".xpm") == 0);
}

int	parse_texture_path(char **dest, const char *textures_path)
{
	const char	*path;
	char		*copy;
	int		fd;

	if (!dest || !textures_path)
		return (-1);
	path = textures_path + 2;
	while (*path == ' ' || *path == '\t')
		path++;
	if (*path == '\0' || *path == '\n')
		return (-1);
	copy = ft_strdup(path);
	if (!copy)
		return (-1);
	strip_newline(copy);
	if (check_texture_path(copy))
		return (free(copy), -1);
	if (!check_texture_extension(copy))
		return (free(copy), -1);
	fd = open(copy, O_RDONLY);
	if (fd < 0)
		return (free(copy), -1);
	close(fd);
	*dest = copy;
	return (0);
}
