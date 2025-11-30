/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <victor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:57 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/11/30 20:54:59 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	drain_gnl_fd(int fd)
{
	char	*tmp;

	if (fd < 0)
		return ;
	tmp = get_next_line(fd);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
}

void	free_config(t_game *g)
{
	if (!g)
		return ;
	if (g->mlx)
	{
		if (g->texture_no.imagen)
		{
			mlx_destroy_image(g->mlx, g->texture_no.imagen);
			g->texture_no.imagen = NULL;
			g->texture_no.addr = NULL;
		}
		if (g->texture_so.imagen)
		{
			mlx_destroy_image(g->mlx, g->texture_so.imagen);
			g->texture_so.imagen = NULL;
			g->texture_so.addr = NULL;
		}
		if (g->texture_we.imagen)
		{
			mlx_destroy_image(g->mlx, g->texture_we.imagen);
			g->texture_we.imagen = NULL;
			g->texture_we.addr = NULL;
		}
		if (g->texture_ea.imagen)
		{
			mlx_destroy_image(g->mlx, g->texture_ea.imagen);
			g->texture_ea.imagen = NULL;
			g->texture_ea.addr = NULL;
		}
		if (g->frame_img)
		{
			mlx_destroy_image(g->mlx, g->frame_img);
			g->frame_img = NULL;
			g->frame_addr = NULL;
		}
		if (g->window)
			(mlx_destroy_window(g->mlx, g->window), g->window = NULL);
		(mlx_destroy_display(g->mlx), free(g->mlx), g->mlx = NULL);
	}
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

int	extension_is_cub(const char *fname)
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
	if (flag_walkable == 1)
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

int	find_player_in_copy(char **copy, int map_height, int *out_y, int *out_x)
{
	int	y;
	int	x;

	if (!copy || !out_y || !out_x)
		return (-1);
	y = 0;
	while (y < map_height)
	{
		x = 0;
		while (copy[y][x])
		{
			if (copy[y][x] == 'N' || copy[y][x] == 'S'
				|| copy[y][x] == 'E' || copy[y][x] == 'W')
			{
				*out_y = y;
				*out_x = x;
				return (0);
			}
			x++;
		}
		y++;
	}
	return (-1);
}

int	check_map_closed(t_game *g)
{
	int		py;
	int		px;
	char	**copy;
	int		y;
	int		x;

	copy = copy_map(g);
	if (!copy)
		return (-1);
	if (find_player_in_copy(copy, g->map_height, &py, &px) < 0)
		return (free_copy(copy), write(2, "Error\nPlayer not found\n", 23), -1);
	if (flood_fill(g, copy, py, px) < 0)
		return (free_copy(copy), write(2, "Error\nMap not closed\n", 21), -1);
	y = 0;
	while (copy[y])
	{
		x = -1;
		while (copy[y][++x])
			if (check_char(copy[y][x], 1))
				return (free_copy(copy),
					write(2, "Error\nArea inaccessible\n", 24), -1);
		y++;
	}
	return (free_copy(copy), 0);
}

/*int	check_map_closed(t_game *g)
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
	return (free_copy(copy), write(2, "Error\nInvalid map\n", 18), -1);
}*/

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
	line = get_next_line(fd);
	while (line != NULL)
	{
		tmp = ft_strjoin(all, line);
		free(all);
		free(line);
		if (!tmp)
			return (NULL);
		all = tmp;
		line = get_next_line(fd);
	}
	return (all);
}

/*int	parse_map(int fd, char *first_line, t_game *g)
{
	char	*rest;
	char	*big_map;
	int		len;

	if (!first_line || !g)
		return (-1);
	rest = read_rest_of_file(fd);
	if (!rest)
		return (free(first_line), -1);
	big_map = ft_strjoin(first_line, rest);
	(free(first_line), free(rest));
	if (!big_map)
		return (-1);
	g->map = ft_split(big_map, '\n');
	free(big_map);
	if (!g->map)
		return (-1);
	while (g->map[g->map_height])
	{
		len = ft_strlen(g->map[g->map_height]);
		if (len > g->map_width)
			g->map_width = len;
		g->map_height++;
	}
	return (0);
}*/

/*int parse_map(int fd, char *first_line, t_game *g)
{
	char	*line;
	char	*big_map;
	char	*rest;
	int		idx;

	if (!first_line || !g)
		return (-1);
	rest = read_rest_of_file(fd);
	if (!rest)
		return (free(first_line), -1);
	big_map = ft_strjoin(first_line, rest);
	free(first_line);
	free(rest);
	if (!big_map)
		return (-1);
	g->map_height = 0;
	idx = 0;
	while (big_map[idx])
		if (big_map[idx++] == '\n')
			g->map_height++;
	if (big_map[idx - 1] != '\n') // si no termina en \n
		g->map_height++;
	g->map = malloc(sizeof(char *) * (g->map_height + 1));
	if (!g->map)
		return (free(big_map), -1);
	idx = 0;
	line = big_map;
	int line_start = 0;
	int y = 0;
	while (line[idx])
	{
		if (line[idx] == '\n')
		{
			int len = idx - line_start;
			g->map[y] = malloc(len + 1);
			if (!g->map[y])
			{
				while (--y >= 0)
					free(g->map[y]);
				free(g->map);
				free(big_map);
				return (-1);
			}
			if (len)
				memcpy(g->map[y], line + line_start, len);
			g->map[y][len] = '\0';
			line_start = idx + 1;
			y++;
		}
		idx++;
	}
	if (line_start < idx)
	{
		int len = idx - line_start;
		g->map[y] = malloc(len + 1);
		if (!g->map[y])
		{
			while (--y >= 0)
				free(g->map[y]);
			free(g->map);
			free(big_map);
			return (-1);
		}
		memcpy(g->map[y], line + line_start, len);
		g->map[y][len] = '\0';
		y++;
	}
	g->map[y] = NULL;
	g->map_width = 0;
	for (y = 0; g->map[y]; y++)
	{
		int len = ft_strlen(g->map[y]);
		if (len > g->map_width)
			g->map_width = len;
	}
	free(big_map);
	return (0);
}*/

int	parse_map(int fd, char *first_line, t_game *g)
{
	char	*line;
	char	*big_map;
	char	*rest;
	int		idx;
	int		line_start;
	int		y;
	int		len;

	if (!first_line || !g)
		return (perror("Error\nStruct\n"), -1);
	rest = read_rest_of_file(fd);
	if (!rest)
		return (perror("Error\nRead\n"), free(first_line), -1);
	big_map = ft_strjoin(first_line, rest);
	(free(first_line), free(rest));
	if (!big_map)
		return (perror("Error\nJoin Map\n"), -1);
	g->map_height = 0;
	idx = 0;
	while (big_map[idx])
		if (big_map[idx++] == '\n')
			g->map_height++;
	if (big_map[idx - 1] != '\n')
		g->map_height++;
	g->map = malloc(sizeof(char *) * (g->map_height + 1));
	if (!g->map)
		return (perror("Error\nMalloc\n"), free(big_map), -1);
	idx = 0;
	line = big_map;
	line_start = 0;
	y = 0;
	while (line[idx])
	{
		if (line[idx] == '\n')
		{
			len = idx - line_start;
			g->map[y] = malloc(len + 1);
			if (!g->map[y])
			{
				while (--y >= 0)
					free(g->map[y]);
				return (perror("Error\nInvalid map\n"), free(g->map), free(big_map), -1);
			}
			if (len)
				ft_memcpy(g->map[y], line + line_start, len);
			g->map[y][len] = '\0';
			line_start = idx + 1;
			y++;
		}
		idx++;
	}
	if (line_start < idx)
	{
		len = idx - line_start;
		g->map[y] = malloc(len + 1);
		if (!g->map[y])
		{
			while (--y >= 0)
				free(g->map[y]);
			return (perror("Error\nInvalid map\n"), free(g->map), free(big_map), -1);
		}
		ft_memcpy(g->map[y], line + line_start, len);
		g->map[y][len] = '\0';
		y++;
	}
	g->map[y] = NULL;
	g->map_width = 0;
	y = 0;
	while (g->map[y])
	{
		len = ft_strlen(g->map[y]);
		if (len > g->map_width)
			g->map_width = len;
		y++;
	}
	return (free(big_map), 0);
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

int	check_empty_lines_in_map(t_game *g)
{
	int	y;
	int	in_map;

	if (!g || !g->map)
		return (-1);
	in_map = 0;
	y = 0;
	while (y < g->map_height)
	{
		if (!is_blank_line(g->map[y]))
			in_map = 1;
		else if (in_map)
		{
			write(2, "Error\nEmpty line in map\n", 25);
			return (-1);
		}
		y++;
	}
	return (0);
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

/*int	parse_one_header_ordered(const char *line, t_game *g, int *order)
{
	char	*trim;
	int		idx = -1;

	trim = ft_strtrim(line, " \t\n\r");
	if (!trim)
		return (-1);
	if (ft_strncmp(trim, "NO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 0;
	else if (ft_strncmp(trim, "SO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 1;
	else if (ft_strncmp(trim, "WE", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 2;
	else if (ft_strncmp(trim, "EA", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 3;
	else if (trim[0] == 'F' && (trim[1] == ' ' || trim[1] == '\t'))
		idx = 4;
	else if (trim[0] == 'C' && (trim[1] == ' ' || trim[1] == '\t'))
		idx = 5;
	if (idx == -1)
		return (free(trim), -1);
	// Verificar que el orden sea correcto
	for (int i = 0; i < idx; i++)
	{
		if (!order[i])
		{
			// error: se encontró idx antes de un elemento anterior
			free(trim);
			return (-1);
		}
	}
	// Procesar realmente la línea
	if (idx == 0)
	{
		if (g->has_no || set_texture(&g->no, trim + 2) < 0)
		return (free(trim), -1);
		g->has_no = 1;
	}
	else if (idx == 1)
	{
		if (g->has_so || set_texture(&g->so, trim + 2) < 0)
		return (free(trim), -1);
		g->has_so = 1;
	}
	else if (idx == 2)
	{
		if (g->has_we || set_texture(&g->we, trim + 2) < 0)
		return (free(trim), -1);
		g->has_we = 1;
	}
	else if (idx == 3)
	{
		if (g->has_ea || set_texture(&g->ea, trim + 2) < 0)
		return (free(trim), -1);
		g->has_ea = 1;
	}
	else if (idx == 4)
	{
		if (g->has_floor || parse_color_values(trim + 1, g->floor) < 0)
		return (free(trim), -1);
		g->has_floor = 1;
	}
	else if (idx == 5)
	{
		if (g->has_ceiling || parse_color_values(trim + 1, g->ceiling) < 0)
		return (free(trim), -1);
		g->has_ceiling = 1;
	}
	order[idx] = 1;
	free(trim);
	return (0);
}*/

int	parse_one_header_ordered(const char *line, t_game *g, int *order)
{
	char	*trim;
	int		idx;
	int		i;

	i = 0;
	idx = -1;
	trim = ft_strtrim(line, " \t\n\r");
	if (!trim)
		return (-1);
	if (ft_strncmp(trim, "NO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 0;
	else if (ft_strncmp(trim, "SO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 1;
	else if (ft_strncmp(trim, "WE", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 2;
	else if (ft_strncmp(trim, "EA", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		idx = 3;
	else if (trim[0] == 'F' && (trim[1] == ' ' || trim[1] == '\t'))
		idx = 4;
	else if (trim[0] == 'C' && (trim[1] == ' ' || trim[1] == '\t'))
		idx = 5;
	if (idx == -1)
		return (free(trim), -1);
	while (i < idx)
	{
		if (!order[i])
			return (free(trim), -1);
		i++;
	}
	if (idx == 0)
	{
		if (g->has_no || set_texture(&g->no, trim + 2) < 0)
			return (free(trim), -1);
		g->has_no = 1;
	}
	else if (idx == 1)
	{
		if (g->has_so || set_texture(&g->so, trim + 2) < 0)
			return (free(trim), -1);
		g->has_so = 1;
	}
	else if (idx == 2)
	{
		if (g->has_we || set_texture(&g->we, trim + 2) < 0)
			return (free(trim), -1);
		g->has_we = 1;
	}
	else if (idx == 3)
	{
		if (g->has_ea || set_texture(&g->ea, trim + 2) < 0)
			return (free(trim), -1);
		g->has_ea = 1;
	}
	else if (idx == 4)
	{
		if (g->has_floor || parse_color_values(trim + 1, g->floor) < 0)
			return (free(trim), -1);
		g->has_floor = 1;
	}
	else if (idx == 5)
	{
		if (g->has_ceiling || parse_color_values(trim + 1, g->ceiling) < 0)
			return (free(trim), -1);
		g->has_ceiling = 1;
	}
	order[idx] = 1;
	return (free(trim), 0);
}

int	parse_headers(int fd, t_game *g, char **out_first_map_line)
{
	char	*line;
	int		order[6];
	int		i;

	i = 0;
	while (i < 6)
		order[i++] = 0;
	if (!g || !out_first_map_line)
		return (-1);
	*out_first_map_line = NULL;
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
			if (parse_one_header_ordered(line, g, order) < 0)
				return (free(line), -1);
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea
			|| !g->has_floor || !g->has_ceiling)
			return (free(line), -1);
		return (*out_first_map_line = line, 0);
	}
	if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea || !g->has_floor
		|| !g->has_ceiling)
		return (-1);
	return (0);
}

int	pad_map(t_game *g, int y)
{
	int		len;
	int		i;
	char	*newrow;

	if (!g || !g->map)
		return (write(2, "Error\nStruct\n", 13), -1);
	y = -1;
	while (++y < g->map_height)
	{
		len = ft_strlen(g->map[y]);
		if (len < g->map_width)
		{
			newrow = malloc(g->map_width + 1);
			if (!newrow)
				return (write(2, "Error\nMalloc\n", 13), -1);
			i = -1;
			while (++i < len)
				newrow[i] = g->map[y][i];
			while (i < g->map_width)
				newrow[i++] = ' ';
			newrow[i] = '\0';
			(free(g->map[y]), g->map[y] = newrow);
		}
	}
	return (0);
}

int	find_player(t_game *g, int *out_y, int *out_x, char *out_orient)
{
	int	y;
	int	x;

	if (!g || !g->map || !out_y || !out_x || !out_orient)
		return (write(2, "Error\nStruct\n", 13), -1);
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
	return (write(2, "Error\nPlayer not found\n", 23), -1);
}

int	key_press(int keycode, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (keycode == 65307)
		return (handle_close(g));
	else if (keycode == 119 || keycode == 'w')
		g->key_w = 1;
	else if (keycode == 115 || keycode == 's')
		g->key_s = 1;
	else if (keycode == 97 || keycode == 'a')
		g->key_a = 1;
	else if (keycode == 100 || keycode == 'd')
		g->key_d = 1;
	else if (keycode == 65361)
		g->key_left = 1;
	else if (keycode == 65363)
		g->key_right = 1;
	return (0);
}

int	handle_close(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	(void)g;
	free_map(g);
	free_config(g);
	exit(0);
	return (0);
}

int	load_texture(void *mlx, t_texture *texture, char *path)
{
	texture->imagen = mlx_xpm_file_to_image(mlx, path,
			&texture->width, &texture->height);
	if (!texture->imagen)
		return (write(2, "Error\nTexture load failed\n", 26), -1);
	texture->addr = mlx_get_data_addr(texture->imagen, &texture->bit_by_pixel,
			&texture->line_len_byte, &texture->endian);
	return (0);
}

int	get_tex_pixel(t_texture *texture, int x, int y)
{
	char	*px;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0);
	px = texture->addr + (y * texture->line_len_byte + x
			* (texture->bit_by_pixel / 8));
	return (*(unsigned int *)px);
}

void	draw_background_to_frame(t_game *g)
{
	int				x;
	int				y;
	unsigned int	color_c;
	unsigned int	color_f;

	if (!g || !g->frame_img || !g->frame_addr)
		return ;
	color_c = (g->ceiling[0] << 16) | (g->ceiling[1] << 8) | g->ceiling[2];
	color_f = (g->floor[0] << 16) | (g->floor[1] << 8) | g->floor[2];
	y = 0;
	while (y < g->screen_h)
	{
		x = 0;
		while (x < g->screen_w)
		{
			if (y < g->screen_h / 2)
				*(unsigned int *)(g->frame_addr + y * g->frame_line_len
						+ x * (g->frame_bpp / 8)) = color_c;
			else
				*(unsigned int *)(g->frame_addr + y * g->frame_line_len
						+ x * (g->frame_bpp / 8)) = color_f;
			x++;
		}
		y++;
	}
}

void	init_player(t_game *g, int py, int px, char orient)
{
	g->posx = (double)px + 0.5;
	g->posy = (double)py + 0.5;
	if (orient == 'N')
	{
		g->dirx = 0.0;
		g->diry = -1.0;
		g->planex = 0.66;
		g->planey = 0.0;
	}
	else if (orient == 'S')
	{
		g->dirx = 0.0;
		g->diry = 1.0;
		g->planex = -0.66;
		g->planey = 0.0;
	}
	else if (orient == 'E')
	{
		g->dirx = 1.0;
		g->diry = 0.0;
		g->planex = 0.0;
		g->planey = 0.66;
	}
	else if (orient == 'W')
	{
		g->dirx = -1.0;
		g->diry = 0.0;
		g->planex = 0.0;
		g->planey = -0.66;
	}
}

int	create_frame(t_game *g)
{
	if (g->frame_img)
		mlx_destroy_image(g->mlx, g->frame_img);
	g->frame_img = mlx_new_image(g->mlx, g->screen_w, g->screen_h);
	if (!g->frame_img)
		return (write(2, "Error\nMalloc frame\n", 19), -1);
	g->frame_addr = mlx_get_data_addr(g->frame_img,
			&g->frame_bpp, &g->frame_line_len, &g->frame_endian);
	return (0);
}

double	get_time_s(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

int	key_release(int keycode, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (keycode == 119 || keycode == 'w')
		g->key_w = 0;
	else if (keycode == 115 || keycode == 's')
		g->key_s = 0;
	else if (keycode == 97 || keycode == 'a')
		g->key_a = 0;
	else if (keycode == 100 || keycode == 'd')
		g->key_d = 0;
	else if (keycode == 65361)
		g->key_left = 0;
	else if (keycode == 65363)
		g->key_right = 0;
	return (0);
}

/*void	update_player(t_game *g, double delta)
{
	double	move_speed;
	double	rot_speed;
	double	nx;
	double	ny;
	double	old_dir_x;
	double	old_plane_x;

	move_speed = 3.0 * delta;
	rot_speed  = 2.0 * delta;
	if (g->key_w)
	{
		nx = g->posx + g->dirx * move_speed;
		ny = g->posy + g->diry * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_s)
	{
		nx = g->posx - g->dirx * move_speed;
		ny = g->posy - g->diry * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_a)
	{
		nx = g->posx - g->planex * move_speed;
		ny = g->posy - g->planey * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_d)
	{
		nx = g->posx + g->planex * move_speed;
		ny = g->posy + g->planey * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_left)
	{
		old_dir_x = g->dirx;
		g->dirx = g->dirx * cos(-rot_speed) - g->diry * sin(-rot_speed);
		g->diry = old_dir_x * sin(-rot_speed) + g->diry * cos(-rot_speed);
		old_plane_x = g->planex;
		g->planex = g->planex * cos(-rot_speed) - g->planey * sin(-rot_speed);
		g->planey = old_plane_x * sin(-rot_speed) + g->planey * cos(-rot_speed);
	}
	if (g->key_right)
	{
		old_dir_x = g->dirx;
		g->dirx = g->dirx * cos(rot_speed) - g->diry * sin(rot_speed);
		g->diry = old_dir_x * sin(rot_speed) + g->diry * cos(rot_speed);
		old_plane_x = g->planex;
		g->planex = g->planex * cos(rot_speed) - g->planey * sin(rot_speed);
		g->planey = old_plane_x * sin(rot_speed) + g->planey * cos(rot_speed);
	}
}*/

void	update_player(t_game *g, double delta)
{
	double	move_speed;
	double	rot_speed;
	double	nx;
	double	ny;
	double	old_dir_x;
	double	old_plane_x;

	move_speed = 3.0 * delta;
	rot_speed  = 2.0 * delta;
	if (g->key_w)
	{
		nx = g->posx + g->dirx * move_speed;
		ny = g->posy + g->diry * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_s)
	{
		nx = g->posx - g->dirx * move_speed;
		ny = g->posy - g->diry * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_a)
	{
		nx = g->posx - g->planex * move_speed;
		ny = g->posy - g->planey * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_d)
	{
		nx = g->posx + g->planex * move_speed;
		ny = g->posy + g->planey * move_speed;
		if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
			&& g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
			g->posy = ny;
		if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
			&& g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
			g->posx = nx;
	}
	if (g->key_left || g->key_right)
	{
		if (g->key_left)
			rot_speed = -rot_speed;
		old_dir_x = g->dirx;
		g->dirx = g->dirx * cos(rot_speed) - g->diry * sin(rot_speed);
		g->diry = old_dir_x * sin(rot_speed) + g->diry * cos(rot_speed);
		old_plane_x = g->planex;
		g->planex = g->planex * cos(rot_speed) - g->planey * sin(rot_speed);
		g->planey = old_plane_x * sin(rot_speed) + g->planey * cos(rot_speed);
	}
}

void	put_pixel_frame(t_game *g, int x, int y, unsigned int color)
{
	char	*dst;

	if (!g || !g->frame_addr)
		return ;
	if (x < 0 || x >= g->screen_w || y < 0 || y >= g->screen_h)
		return ;
	dst = g->frame_addr + y * g->frame_line_len + x * (g->frame_bpp / 8);
	*(unsigned int *)dst = color;
}

t_texture	*choose_wall_texture(t_game *g, int side, double ray_dir_x, double ray_dir_y)
{
	if (!g)
		return (NULL);
	if (side == 0)
	{
		if (ray_dir_x > 0)
			return (&g->texture_we);
		else
			return (&g->texture_ea);
	}
	else
	{
		if (ray_dir_y > 0)
			return (&g->texture_no);
		else
			return (&g->texture_so);
	}
}
/* 
void	render_frame(t_game *g)
{
	int	x;
	int	map_x;
	int	map_y;
	int	step_x;
	int	step_y;
	int	hit;
	int	side;
	int	line_height;
	int	draw_start;
	int	draw_end;
	int	tex_width;
	int	tex_height;
	int	tex_x;
	int	y;
	int	tex_y;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	double	wall_x;
	double	step;
	double	tex_pos;
	double	denom;
	t_texture	*tex;
	unsigned int	color;
	char	mch;

	if (!g || !g->frame_img || !g->frame_addr)
		return ;
	x = 0;
	while (x < g->screen_w)
	{
		camera_x = 2.0 * x / (double)g->screen_w - 1.0;
		ray_dir_x = g->dirx + g->planex * camera_x;
		ray_dir_y = g->diry + g->planey * camera_x;
		map_x = (int)g->posx;
		map_y = (int)g->posy;
		if (ray_dir_x == 0.0)
			delta_dist_x = 1e30;
		else
			delta_dist_x = fabs(1.0 / ray_dir_x);
		if (ray_dir_y == 0.0)
			delta_dist_y = 1e30;
		else
			delta_dist_y = fabs(1.0 / ray_dir_y);
		hit = 0;
		side = 0;
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (g->posx - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - g->posx) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (g->posy - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - g->posy) * delta_dist_y;
		}
		while (!hit)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (map_y < 0 || map_y >= g->map_height || map_x < 0 || map_x >= g->map_width)
			{
				hit = 1;
				break ;
			}
			mch = g->map[map_y][map_x];
			if (mch == '1')
				hit = 1;
			else if (mch == ' ')
				hit = 1;
		}
		if (side == 0)
		{
			if (ray_dir_x == 0)
				denom = 1e-6;
			else
				denom = ray_dir_x;
			perp_wall_dist = (map_x - g->posx + (1 - step_x) / 2.0) / denom;
		}
		else
		{
			if (ray_dir_y == 0)
				denom = 1e-6;
			else
				denom = ray_dir_y;
			perp_wall_dist = (map_y - g->posy + (1 - step_y) / 2.0) / denom;
		}
		if (perp_wall_dist <= 0.0)
			perp_wall_dist = 1e-6;
		line_height = (int)(g->screen_h / perp_wall_dist);
		draw_start = -line_height / 2 + g->screen_h / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = line_height / 2 + g->screen_h / 2;
		if (draw_end >= g->screen_h)
			draw_end = g->screen_h - 1;
		if (side == 0)
			wall_x = g->posy + perp_wall_dist * ray_dir_y;
		else
			wall_x = g->posx + perp_wall_dist * ray_dir_x;
		wall_x -= floor(wall_x);
		tex = choose_wall_texture(g, side, ray_dir_x, ray_dir_y);
		if (!tex || !tex->addr || tex->width <= 0 || tex->height <= 0)
		{
			x++;
			continue ;
		}
		tex_width = tex->width;
		tex_height = tex->height;
		tex_x = (int)(wall_x * (double)tex_width);
		if ((side == 0 && ray_dir_x > 0) || (side == 1 && ray_dir_y < 0))
			tex_x = tex_width - tex_x - 1;
		if (tex_x < 0)
			tex_x = 0;
		if (tex_x >= tex_width)
			tex_x = tex_width - 1;
		step = 1.0 * tex_height / (double)line_height;
		tex_pos = (draw_start - g->screen_h / 2 + line_height / 2) * step;
		y = draw_start;
		while (y <= draw_end)
		{
			tex_y = (int)tex_pos;
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= tex_height)
				tex_y = tex_height - 1;
			tex_pos += step;
			color = (unsigned int)get_tex_pixel(tex, tex_x, tex_y);
			if (side == 1)
				color = ((color & 0xFEFEFE) >> 1);
			put_pixel_frame(g, x, y, color);
			y++;
		}
		x++;
	}
} */

void	init_render_vars(t_game *g, t_render *r, int x)
{
	r->x = x;
	r->camera_x = 2.0 * x / (double)g->screen_w - 1.0;
	r->ray_dir_x = g->dirx + g->planex * r->camera_x;
	r->ray_dir_y = g->diry + g->planey * r->camera_x;
	r->map_x = (int)g->posx;
	r->map_y = (int)g->posy;
	if (r->ray_dir_x == 0.0)
		r->delta_dist_x = 1e30;
	else
		r->delta_dist_x = fabs(1.0 / r->ray_dir_x);
	if (r->ray_dir_y == 0.0)
		r->delta_dist_y = 1e30;
	else
		r->delta_dist_y = fabs(1.0 / r->ray_dir_y);
	r->hit = 0;
	r->side = 0;
}

void	init_steps(t_game *g, t_render *r)
{
	if (r->ray_dir_x < 0)
	{
		r->step_x = -1;
		r->side_dist_x = (g->posx - r->map_x) * r->delta_dist_x;
	}
	else
	{
		r->step_x = 1;
		r->side_dist_x = (r->map_x + 1.0 - g->posx) * r->delta_dist_x;
	}
	if (r->ray_dir_y < 0)
	{
		r->step_y = -1;
		r->side_dist_y = (g->posy - r->map_y) * r->delta_dist_y;
	}
	else
	{
		r->step_y = 1;
		r->side_dist_y = (r->map_y + 1.0 - g->posy) * r->delta_dist_y;
	}
}

void	perform_dda(t_game *g, t_render *r)
{
	while (!r->hit)
	{
		if (r->side_dist_x < r->side_dist_y)
		{
			r->side_dist_x += r->delta_dist_x;
			r->map_x += r->step_x;
			r->side = 0;
		}
		else
		{
			r->side_dist_y += r->delta_dist_y;
			r->map_y += r->step_y;
			r->side = 1;
		}
		if (r->map_y < 0 || r->map_y >= g->map_height
			|| r->map_x < 0 || r->map_x >= g->map_width)
		{
			r->hit = 1;
			break ;
		}
		r->mch = g->map[r->map_y][r->map_x];
		if (r->mch == '1' || r->mch == ' ')
			r->hit = 1;
	}
}

void	compute_projection(t_game *g, t_render *r)
{
	if (r->side == 0)
	{
		if (r->ray_dir_x == 0)
			r->denom = 1e-6;
		else
			r->denom = r->ray_dir_x;
		r->perp_wall_dist = (r->map_x - g->posx + (1 - r->step_x) / 2.0) / r->denom;
	}
	else
	{
		if (r->ray_dir_y == 0)
			r->denom = 1e-6;
		else
			r->denom = r->ray_dir_y;
		r->perp_wall_dist = (r->map_y - g->posy + (1 - r->step_y) / 2.0) / r->denom;
	}
	if (r->perp_wall_dist <= 0.0)
		r->perp_wall_dist = 1e-6;
	r->line_height = (int)(g->screen_h / r->perp_wall_dist);
	r->draw_start = -r->line_height / 2 + g->screen_h / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	r->draw_end = r->line_height / 2 + g->screen_h / 2;
	if (r->draw_end >= g->screen_h)
		r->draw_end = g->screen_h - 1;
}

void	render_column(t_game *g, t_render *r, t_texture *tex)
{
	r->tex_x = (int)(r->wall_x * (double)r->tex_width);
	if ((r->side == 0 && r->ray_dir_x > 0)
		|| (r->side == 1 && r->ray_dir_y < 0))
		r->tex_x = r->tex_width - r->tex_x - 1;
	if (r->tex_x < 0)
		r->tex_x = 0;
	if (r->tex_x >= r->tex_width)
		r->tex_x = r->tex_width - 1;
	r->step = 1.0 * r->tex_height / (double)r->line_height;
	r->tex_pos = (r->draw_start - g->screen_h / 2 + r->line_height / 2) * r->step;
	while (r->draw_start <= r->draw_end)
	{
		r->tex_y = (int)r->tex_pos;
		if (r->tex_y < 0)
			r->tex_y = 0;
		if (r->tex_y >= r->tex_height)
			r->tex_y = r->tex_height - 1;
		r->tex_pos += r->step;
		r->color = get_tex_pixel(tex, r->tex_x, r->tex_y);
		if (r->side == 1)
			r->color = ((r->color & 0xFEFEFE) >> 1);
		put_pixel_frame(g, r->x, r->draw_start, r->color);
		r->draw_start++;
	}
}

void	render_frame(t_game *g, t_render *r, t_texture **tex)
{
	if (!g || !g->frame_img || !g->frame_addr)
		return ;
	r->x = 0;
	while (r->x < g->screen_w)
	{
		init_render_vars(g, r, r->x);
		init_steps(g, r);
		perform_dda(g, r);
		compute_projection(g, r);
		if (r->side == 0)
			r->wall_x = g->posy + r->perp_wall_dist * r->ray_dir_y;
		else
			r->wall_x = g->posx + r->perp_wall_dist * r->ray_dir_x;
		r->wall_x -= floor(r->wall_x);
		*tex = choose_wall_texture(g, r->side, r->ray_dir_x, r->ray_dir_y);
		if (!(*tex) || !(*tex)->addr || (*tex)->width <= 0)
		{
			r->x++;
			continue;
		}
		r->tex_width  = (*tex)->width;
		r->tex_height = (*tex)->height;
		render_column(g, r, *tex);
		r->x++;
	}
}

/* ---------------------------------------------------------------------- */

int	game_loop(void *param)
{
	static double	last;
	double			delta;
	t_game			*g;
	t_render			r;
	t_texture			*tex;

	g = (t_game *)param;
	g->now = get_time_s();
	if (!last)
		last = g->now;
	delta = g->now - last;
	if (delta > 0.25)
		delta = 0.25;
	last = g->now;
	update_player(g, delta);
	if (!g->frame_img)
		if (create_frame(g) < 0)
			return (write(2, "Error\ncreate_frame failed\n", 26), 1);
	draw_background_to_frame(g);
	render_frame(g, &r, &tex);
	mlx_put_image_to_window(g->mlx, g->window, g->frame_img, 0, 0);
	return (0);
}

int	mouse_move(int x, int y, void *param)
{
	t_game	*g;
	int		dx;
	double	angle;
	double	old_dir_x;
	double	old_plane_x;

	g = (t_game *)param;
	if (!g)
		return (0);
	dx = x - g->mouse_last_x;
	g->mouse_last_x = x;
	g->mouse_last_y = y;
	if (dx > -2 && dx < 2)
		return (0);
	if (dx > 50)
		dx = 50;
	if (dx < -50)
		dx = -50;
	angle = dx * 0.003;
	old_dir_x = g->dirx;
	g->dirx = g->dirx * cos(angle) - g->diry * sin(angle);
	g->diry = old_dir_x * sin(angle) + g->diry * cos(angle);
	old_plane_x = g->planex;
	g->planex = g->planex * cos(angle) - g->planey * sin(angle);
	return (g->planey = old_plane_x * sin(angle) + g->planey * cos(angle), 0);
}

void	init_struct_g(t_game *g)
{
	if (!g)
		return ;
	ft_memset(g, 0, sizeof(*g));
	g->screen_w = 1920;
	g->screen_h = 1080;
	g->mlx = mlx_init();
}

/*int	main(int argc, char **argv)
{
	int			fd;
	int			fd_empty;
	int			py;
	int			px;
	int			empty_file;
	t_game		g;
	char		*first_map_line;
	char		orient;
	char		tmp_empty_file[1];

	if (argc != 2)
		return (write(2, "Error\nArguments\n", 16), 1);
	if (!extension_is_cub(argv[1]))
		return (write(2, "Error\nExtension\n", 16), 1);
	fd_empty = open(argv[1], O_RDONLY);
	if (fd_empty < 0)
		return (perror("Error\nOpen"), 1);
	empty_file = read(fd_empty, tmp_empty_file, 1);
	if (empty_file < 0)
		return (close(fd_empty), perror("Error\nRead"), 1);
	if (empty_file == 0)
		return (close(fd_empty), write(2, "Error\nEmpty file\n", 17), 1);
	close(fd_empty);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Error\nOpen"), 1);
	init_struct_g(&g);
	if (!g.mlx)
		return (close(fd), write(2, "Error\nmlx_init failed\n", 22),
			free_map(&g), free_config(&g), 1);
	first_map_line = NULL;
	if (parse_headers(fd, &g, &first_map_line) < 0)
	{
		(drain_gnl_fd(fd), close(fd));
		return (free_config(&g), write(2, "Error\nHeader\n", 13), 1);
	}
	if (parse_map(fd, first_map_line, &g) < 0)
	{
		if (first_map_line)
			free(first_map_line);
		close(fd);
		return (free_config(&g), write(2, "Error\nInvalid map\n", 18), 1);
	}
	if (check_empty_lines_in_map(&g) < 0)
		return (free_map(&g), free_config(&g), close(fd), 1);
	if (pad_map(&g) < 0)
		return (close(fd), free_map(&g), free_config(&g),
			write(2, "Error\nMalloc\n", 13), 1);
	if (validate_map_chars(&g) < 0 || check_map_closed(&g) < 0)
		return (close(fd), free_map(&g), free_config(&g), 1);
	if (find_player(&g, &py, &px, &orient) < 0)
		return (close(fd), write(2, "Error\nPlayer not found\n", 23),
			free_map(&g), free_config(&g), 1);
	close(fd);
	if (load_texture(g.mlx, &g.texture_no, g.no) < 0
		|| load_texture(g.mlx, &g.texture_so, g.so) < 0
		|| load_texture(g.mlx, &g.texture_we, g.we) < 0
		|| load_texture(g.mlx, &g.texture_ea, g.ea) < 0)
		return (write(2, "Error\nTexture load failed\n", 26),
			free_map(&g), free_config(&g), 1);
	g.window = mlx_new_window(g.mlx, g.screen_w, g.screen_h, "cub3D");
	if (!g.window)
		return (write(2, "Error\nmlx_new_window failed\n", 28),
			free_map(&g), free_config(&g), 1);
	mlx_hook(g.window, 6, 64, mouse_move, &g);
	init_player(&g, py, px, orient);
	if (create_frame(&g) < 0)
		return (write(2, "Error\nMalloc frame\n", 19),
			free_map(&g), free_config(&g), 1);
	mlx_hook(g.window, 2, 1, key_press, &g);
	mlx_hook(g.window, 3, 2, key_release, &g);
	mlx_hook(g.window, 17, 0, handle_close, &g);
	mlx_loop_hook(g.mlx, game_loop, &g);
	mlx_loop(g.mlx);
	return (free_map(&g), free_config(&g), 0);
}*/

int	parse(char **argv, t_game *g, int *py, int *px, char *orient)
{
	int		fd;
	int		fd_empty;
	int		empty_file;
	char	tmp_empty_file[1];
	char	*first_map_line;

	first_map_line = NULL;
	if (!extension_is_cub(argv[1]))
		return (write(2, "Error\nExtension\n", 16), -1);
	fd_empty = open(argv[1], O_RDONLY);
	if (fd_empty < 0)
		return (perror("Error\nOpen"), -1);
	empty_file = read(fd_empty, tmp_empty_file, 1);
	if (empty_file < 0)
		return (close(fd_empty), perror("Error\nRead"), -1);
	if (empty_file == 0)
		return (close(fd_empty), perror("Error\nEmpty file\n"), -1);
	(close(fd_empty), fd = open(argv[1], O_RDONLY));
	if (fd < 0)
		return (perror("Error\nOpen"), -1);
	init_struct_g(g);
	if (!g->mlx)
		return (close(fd), perror("Error\nmlx_init failed\n"),
			free_map(g), free_config(g), -1);
	if (parse_headers(fd, g, &first_map_line) < 0)
		return (drain_gnl_fd(fd), close(fd), free_config(g),
			write(2, "Error\nHeader\n", 13), -1);
	if (parse_map(fd, first_map_line, g) < 0)
		return (close(fd), free_config(g), -1);
	if (check_empty_lines_in_map(g) < 0 || pad_map(g, 0) < 0 || validate_map_chars(g) < 0
		|| check_map_closed(g) < 0 || find_player(g, py, px, orient) < 0)
		return (free_map(g), free_config(g), close(fd), -1);
	return (close(fd), 0);
}

int	main(int argc, char **argv)
{
	int			py;
	int			px;
	t_game		g;
	char		orient;

	if (argc != 2)
		return (write(2, "Error\nArguments\n", 16), -1);
	if (parse(argv, &g, &py, &px, &orient) < 0)
		return (1);
	if (load_texture(g.mlx, &g.texture_no, g.no) < 0 || load_texture(g.mlx,
			&g.texture_so, g.so) < 0 || load_texture(g.mlx, &g.texture_we,
			g.we) < 0 || load_texture(g.mlx, &g.texture_ea, g.ea) < 0)
		return (free_map(&g), free_config(&g), 1);
	g.window = mlx_new_window(g.mlx, g.screen_w, g.screen_h, "cub3D");
	if (!g.window)
		return (perror("Error\nWindow\n"), free_map(&g), free_config(&g), 1);
	mlx_hook(g.window, 6, 64, mouse_move, &g);
	init_player(&g, py, px, orient);
	if (create_frame(&g) < 0)
		return (free_map(&g), free_config(&g), 1);
	mlx_hook(g.window, 2, 1, key_press, &g);
	mlx_hook(g.window, 3, 2, key_release, &g);
	mlx_hook(g.window, 17, 0, handle_close, &g);
	(mlx_loop_hook(g.mlx, game_loop, &g), mlx_loop(g.mlx));
	return (free_map(&g), free_config(&g), 0);
}
