/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:41:06 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/02 17:46:01 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	strip_nl(char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

int	grow_lines(char ***lines, size_t *cap, size_t count, char *line)
{
	size_t	newcap;
	char	**tmp;
	size_t	i;

	newcap = (*cap) * 2;
	tmp = malloc(sizeof(char *) * newcap);
	if (!tmp)
	{
		free(line);
		while (count--)
			free((*lines)[count]);
		free(*lines);
		perror("Error\nMalloc\n");
		return (-1);
	}
	i = 0;
	while (i < count)
	{
		tmp[i] = (*lines)[i];
		i++;
	}
	free(*lines);
	*lines = tmp;
	*cap = newcap;
	return (0);
}

int	build_map_from_lines(t_game *g, char **lines, size_t count)
{
	size_t	i;

	g->map_height = (int)count;
	g->map = malloc(sizeof(char *) * (g->map_height + 1));
	if (!g->map)
	{
		while (count--)
			free(lines[count]);
		free(lines);
		perror("Error\nMalloc\n");
		return (-1);
	}
	i = 0;
	g->y = 0;
	g->map_width = 0;
	while (i < count)
	{
		strip_nl(lines[i]);
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
	char	*line;
	size_t	cap;
	size_t	count;

	if (!first_line || !g)
		return (perror("Error\nStruct\n"), -1);
	cap = 64;
	lines = malloc(sizeof(char *) * cap);
	if (!lines)
		return (free(first_line), perror("Error\nMalloc\n"), -1);
	count = 0;
	lines[count++] = first_line;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (count >= cap)
			if (grow_lines(&lines, &cap, count, line) < 0)
				return (-1);
		lines[count++] = line;
		line = get_next_line(fd);
	}
	if (build_map_from_lines(g, lines, count) < 0)
		return (-1);
	return (0);
}
