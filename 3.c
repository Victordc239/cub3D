/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:51:47 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/09 17:49:03 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_blank_line(const char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (*s != ' ' && *s != '\t' && *s != '\r' && *s != '\n')
			return (0);
		s++;
	}
	return (1);
}

int	is_header_line(const char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	if (ft_strncmp(line, "NO", 2) == 0 && (line[2] == ' ' || line[2] == '\t'))
		return (1);
	if (ft_strncmp(line, "SO", 2) == 0 && (line[2] == ' ' || line[2] == '\t'))
		return (1);
	if (ft_strncmp(line, "WE", 2) == 0 && (line[2] == ' ' || line[2] == '\t'))
		return (1);
	if (ft_strncmp(line, "EA", 2) == 0 && (line[2] == ' ' || line[2] == '\t'))
		return (1);
	if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
		return (1);
	if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
		return (1);
	return (0);
}

int	consume_headers_until_map_or_eof(int fd, t_game *g, char **out_first_map_line, int order[6])
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		if (is_blank_line(line))
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (is_header_line(line))
		{
			if (parse_header_order(line, g, order) < 0)
				return (free(line), -1);
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea
			|| !g->has_f || !g->has_c)
			return (free(line), -1);
		return (*out_first_map_line = line, 0);
	}
	return (1);
}

int	parse_headers_loop(int fd, t_game *g, char **out_first_map_line, int order[6])
{
	int	result;

	result = consume_headers_until_map_or_eof(fd, g, out_first_map_line, order);
	if (result == -1)
		return (-1);
	if (result == 0)
		return (0);
	if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea || !g->has_f
		|| !g->has_c)
		return (-1);
	return (0);
}

int	parse_headers(int fd, t_game *g, char **out_first_map_line)
{
	int	order[6];
	int	i;

	i = 0;
	while (i < 6)
		order[i++] = 0;
	if (!g || !out_first_map_line)
		return (-1);
	*out_first_map_line = NULL;
	return (parse_headers_loop(fd, g, out_first_map_line, order));
}
