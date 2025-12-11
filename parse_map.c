/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soraya <soraya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:41:06 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/11 01:24:19 by soraya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	strip_newline(char *map_line)
{
	size_t	len;

	if (!map_line)
		return ;
	len = ft_strlen(map_line);
	if (len > 0 && map_line[len - 1] == '\n')
		map_line[len - 1] = '\0';
}

int	resize_lines(char ***lines, size_t *lines_capacity, size_t line_count, char *map_line)
{
	size_t	new_capacity;
	char	**new_lines;
	size_t	i;

	new_capacity = (*lines_capacity) * 2;
	new_lines = malloc(sizeof(char *) * new_capacity);
	if (!new_lines)
	{
		free(map_line);
		while (line_count--)
			free((*lines)[line_count]);
		free(*lines);
		perror("Error\nMalloc\n");
		return (-1);
	}
	i = 0;
	while (i < line_count)
	{
		new_lines[i] = (*lines)[i];
		i++;
	}
	free(*lines);
	*lines = new_lines;
	*lines_capacity = new_capacity;
	return (0);
}

int	build_map(t_game *g, char **lines, size_t line_count)
{
	size_t	i;

	g->map_height = (int)line_count;
	g->map = malloc(sizeof(char *) * (g->map_height + 1));
	if (!g->map)
	{
		while (line_count--)
			free(lines[line_count]);
		free(lines);
		perror("Error\nMalloc\n");
		return (-1);
	}
	i = 0;
	g->y = 0;
	g->map_width = 0;
	while (i < line_count)
	{
		strip_newline(lines[i]);
		g->map[g->y++] = lines[i];
		if ((int)ft_strlen(lines[i]) > g->map_width)
			g->map_width = ft_strlen(lines[i]);
		i++;
	}
	g->map[g->y] = NULL;
	return (free(lines), 0);
}

int	parse_map(int fd, char *first_line, t_game *g)
{
	char	**lines;
	char	*map_line;
	size_t	lines_capacity;
	size_t	line_count;

	if (!first_line || !g)
		return (perror("Error\nStruct\n"), -1);
	lines_capacity = 64;
	lines = malloc(sizeof(char *) * lines_capacity);
	if (!lines)
		return (free(first_line), perror("Error\nMalloc\n"), -1);
	line_count = 0;
	lines[line_count++] = first_line;
	map_line = get_next_line(fd);
	while (map_line != NULL)
	{
		if (line_count >= lines_capacity)
			if (resize_lines(&lines, &lines_capacity, line_count, map_line) < 0)
				return (-1);
		lines[line_count++] = map_line;
		map_line = get_next_line(fd);
	}
	if (build_map(g, lines, line_count) < 0)
		return (-1);
	return (0);
}
