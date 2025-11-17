/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <victor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:57 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/11/16 16:20:54 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_config(t_game *g)
{
	if (!g)
		return ;
	if (g->no)
		(free(g->no), g->no = NULL);
	if (g->so)
		(free(g->so), g->so = NULL);
	if (g->we)
		(free(g->we), g->we = NULL);
	if (g->ea)
		(free(g->ea), g->ea = NULL);
	g->has_no = 0;
	g->has_so = 0;
	g->has_we = 0;
	g->has_ea = 0;
	g->has_floor = 0;
	g->has_ceiling = 0;
}

int	is_cub(const char *fname)
{
	int	len;

	if (!fname)
		return (0);
	len = ft_strlen(fname);
	if (len < 4)
		return (0);
	return (ft_strcmp(fname + len - 4, ".cub") == 0);
}

int	check_char(char c, int flag_walkable)
{
	if (flag_walkable)
		return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
	else
		return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E'
			|| c == 'W' || c == ' ');
}

int	flood_fill(t_game *g, char **visited, int y, int x)
{
	if (!(y >= 0 && y < g->map_height && x >= 0 && x < ft_strlen(visited[y])))
		return (-1);
	if (visited[y][x] == '1' || visited[y][x] == 'X')
		return (0);
	if (visited[y][x] == ' ')
		return (-1);
	visited[y][x] = 'X';
	if (flood_fill(g, visited, y + 1, x) < 0)
		return (-1);
	if (flood_fill(g, visited, y - 1, x) < 0)
		return (-1);
	if (flood_fill(g, visited, y, x + 1) < 0)
		return (-1);
	if (flood_fill(g, visited, y, x - 1) < 0)
		return (-1);
	return (0);
}

char	**copy_map(t_game *g)
{
	char	**copy;
	int		y;

	copy = malloc(sizeof(char *) * (g->map_height + 1));
	if (!copy)
		return (NULL);
	y = 0;
	while (y < g->map_height)
	{
		copy[y] = ft_strdup(g->map[y]);
		if (!copy[y])
		{
			while (--y >= 0)
				free(copy[y]);
			free(copy);
			return (NULL);
		}
		y++;
	}
	copy[y] = NULL;
	return (copy);
}

void	free_copy(char **copy)
{
	int	i;

	i = 0;
	if (!copy)
		return ;
	while (copy[i])
		free(copy[i++]);
	free(copy);
}

int	check_map_closed(t_game *g)
{
	int		y;
	int		x;
	char	**copy;
	int		result;

	copy = copy_map(g);
	if (!copy)
		return (-1);
	y = 0;
	while (y < g->map_height)
	{
		x = 0;
		while (copy[y][x])
		{
			if (check_char(copy[y][x], 1))
			{
				result = flood_fill(g, copy, y, x);
				free_copy(copy);
				if (result < 0)
					return (write(2, "Error\nMap not closed\n", 21), -1);
				return (0);
			}
			x++;
		}
		y++;
	}
	free_copy(copy);
	return (write(2, "Error\nNo walkable area found\n", 29), -1);
}

int	validate_map_chars(t_game *g)
{
	int	y;
	int	x;
	int	player_count;

	if (!g || !g->map)
		return (-1);
	player_count = 0;
	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			if (!check_char(g->map[y][x], 0))
				return (write(2, "Error\nInvalid character in map\n", 31), -1);
			if (g->map[y][x] == 'N' || g->map[y][x] == 'S'
				|| g->map[y][x] == 'E' || g->map[y][x] == 'W')
				player_count++;
			x++;
		}
		y++;
	}
	if (player_count != 1)
		return (write(2, "Error\nInvalid player count\n", 27), -1);
	return (0);
}

char	*read_rest_of_file(int fd)
{
	char	*line;
	char	*all;
	char	*tmp;

	all = ft_strdup("");
	if (!all)
		return (NULL);
	while ((line = get_next_line(fd)) != NULL)
	{
		tmp = ft_strjoin(all, line);
		free(all);
		free(line);
		if (!tmp)
			return (NULL);
		all = tmp;
	}
	return (all);
}

int	parse_map(int fd, char *first_line, t_game *g)
{
	char	*rest;
	char	*big_map;
	char	*tmp;
	int		len;

	if (!first_line || !g)
		return (-1);
	rest = read_rest_of_file(fd);
	if (!rest)
		return (free(first_line), -1);
	tmp = ft_strjoin(first_line, rest);
	free(first_line);
	free(rest);
	if (!tmp)
		return (-1);
	big_map = tmp;
	g->map = ft_split(big_map, '\n');
	free(big_map);
	if (!g->map)
		return (-1);
	g->map_height = 0;
	g->map_width = 0;
	while (g->map[g->map_height])
	{
		len = ft_strlen(g->map[g->map_height]);
		if (len > g->map_width)
			g->map_width = len;
		g->map_height++;
	}
	return (0);
}

void	free_map(t_game *g)
{
	int	i;

	if (!g || !g->map)
		return ;
	i = 0;
	while (g->map[i])
	{
		free(g->map[i]);
		i++;
	}
	free(g->map);
	g->map = NULL;
}

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

void	strip_nl(char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

int	parse_uint0_255(const char **p, int *out)
{
	long		v;
	const char	*s;

	v = 0;
	s = *p;
	if (!s || !ft_isdigit((unsigned char)*s))
		return (-1);
	while (*s && ft_isdigit((unsigned char)*s))
	{
		v = v * 10 + (*s - '0');
		if (v > 255)
			return (-1);
		s++;
	}
	*out = (int)v;
	*p = s;
	return (0);
}

int	parse_color_values(const char *rest, int out_rgb[3])
{
	const char	*p;

	p = rest;
	while (*p == ' ' || *p == '\t')
		p++;
	if (parse_uint0_255(&p, &out_rgb[0]) < 0)
		return (-1);
	if (*p != ',')
		return (-1);
	p++;
	if (parse_uint0_255(&p, &out_rgb[1]) < 0)
		return (-1);
	if (*p != ',')
		return (-1);
	p++;
	if (parse_uint0_255(&p, &out_rgb[2]) < 0)
		return (-1);
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p != '\0' && *p != '\n')
		return (-1);
	return (0);
}

int	set_texture(char **dst, const char *rest)
{
	const char	*p;
	char		*copy;

	p = rest;
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p == '\0' || *p == '\n')
		return (-1);
	copy = ft_strdup(p);
	if (!copy)
		return (-1);
	strip_nl(copy);
	*dst = copy;
	return (0);
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

int	parse_one_header(const char *line, t_game *g)
{
	char	*trim;

	trim = ft_strtrim(line, " \t\n\r");
	if (!trim)
		return (-1);
	if (ft_strncmp(trim, "NO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
	{
		if (g->has_no || set_texture(&g->no, trim + 2) < 0)
			return (free(trim), -1);
		g->has_no = 1;
		return (free(trim), 0);
	}
	if (ft_strncmp(trim, "SO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
	{
		if (g->has_so || set_texture(&g->so, trim + 2) < 0)
			return (free(trim), -1);
		g->has_so = 1;
		return (free(trim), 0);
	}
	if (ft_strncmp(trim, "WE", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
	{
		if (g->has_we || set_texture(&g->we, trim + 2) < 0)
			return (free(trim), -1);
		g->has_we = 1;
		return (free(trim), 0);
	}
	if (ft_strncmp(trim, "EA", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
	{
		if (g->has_ea || set_texture(&g->ea, trim + 2) < 0)
			return (free(trim), -1);
		g->has_ea = 1;
		return (free(trim), 0);
	}
	if (trim[0] == 'F' && (trim[1] == ' ' || trim[1] == '\t'))
	{
		if (g->has_floor || parse_color_values(trim + 1, g->floor) < 0)
			return (free(trim), -1);
		g->has_floor = 1;
		return (free(trim), 0);
	}
	if (trim[0] == 'C' && (trim[1] == ' ' || trim[1] == '\t'))
	{
		if (g->has_ceiling || parse_color_values(trim + 1, g->ceiling) < 0)
			return (free(trim), -1);
		g->has_ceiling = 1;
		return (free(trim), 0);
	}
	return (free(trim), -1);
}

int	parse_headers(int fd, t_game *g, char **out_first_map_line)
{
	char	*line;

	if (!g || !out_first_map_line)
		return (-1);
	*out_first_map_line = NULL;
	while ((line = get_next_line(fd)) != NULL)
	{
		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}
		if (is_header_line(line))
		{
			if (parse_one_header(line, g) < 0)
				return (free(line), free_config(g), -1);
			free(line);
			continue ;
		}
		*out_first_map_line = line;
		return (0);
	}
	if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea || !g->has_floor || !g->has_ceiling)
		return (free_config(g), -1);
	return (0);
}

//hasta aqui es parse

int	pad_map(t_game *g)
{
	int	y;
	int	len;
	int	i;
	char	*newrow;

	if (!g || !g->map)
		return (-1);
	y = 0;
	while (y < g->map_height)
	{
		len = ft_strlen(g->map[y]);
		if (len < g->map_width)
		{
			newrow = malloc(g->map_width + 1);
			if (!newrow)
			return (-1);
			i = 0;
			while (i < len)
			{
			newrow[i] = g->map[y][i];
			i++;
			}
			while (i < g->map_width)
			newrow[i++] = ' ';
			newrow[i] = '\0';
			free(g->map[y]);
			g->map[y] = newrow;
		}
		y++;
	}
	return (0);
}

int	find_player(t_game *g, int *out_y, int *out_x, char *out_orient)
{
	int	y;
	int	x;

	if (!g || !g->map || !out_y || !out_x || !out_orient)
		return (-1);
	y = 0;
	while (y < g->map_height)
	{
		x = 0;
		while (g->map[y][x])
		{
			if (g->map[y][x] == 'N' || g->map[y][x] == 'S'
			|| g->map[y][x] == 'E' || g->map[y][x] == 'W')
			{
				*out_y = y;
				*out_x = x;
				*out_orient = g->map[y][x];
				g->map[y][x] = '0';
				return (0);
			}
			x++;
		}
		y++;
	}
	return (-1);
}

int main(int argc, char **argv)
{
	int		fd;
	t_game	g;
	char		*first_map_line;
	
	int	py;
	int	px;
	char		orient;

	if (argc != 2)
		return (write(2, "Error\nUsage: ./cub3D <file.cub>\n", 33), 1);
	if (!is_cub(argv[1]))
		return (write(2, "Error\nextension\n", 16), 1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Error\nopen"), 1);
	g.no = g.so = g.we = g.ea = NULL;
	g.has_no = g.has_so = g.has_we = g.has_ea = 0;
	g.has_floor = g.has_ceiling = 0;
	first_map_line = NULL;
	if (parse_headers(fd, &g, &first_map_line) < 0 || parse_map(fd, first_map_line, &g) < 0)
	{
		close(fd);
		free_config(&g);
		return (write(2, "Error\nInvalid map\n", 18), 1);
	}
	if (pad_map(&g) < 0)
		return (close(fd), free_map(&g), free_config(&g), write(2, "Error\nMalloc\n", 13), 1);
	if (validate_map_chars(&g) < 0 || check_map_closed(&g) < 0)
		return (free_map(&g), free_config(&g), 1);
	if (find_player(&g, &py, &px, &orient) < 0)
		return (write(2, "Error\nPlayer not found\n", 23), free_map(&g), free_config(&g), 1);
	close(fd);
	if (validate_map_chars(&g) < 0 || check_map_closed(&g) < 0)
		return (free_map(&g), free_config(&g), 1);
	return (free_map(&g), free_config(&g), 0);
}
