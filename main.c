/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:57 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/12/02 17:30:52 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_image(void *mlx, void **img_ptr, char **addr_ptr)
{
	if (!mlx || !img_ptr)
		return ;
	if (*img_ptr)
	{
		mlx_destroy_image(mlx, *img_ptr);
		*img_ptr = NULL;
		if (addr_ptr)
			*addr_ptr = NULL;
	}
}

void	free_config(t_game *g)
{
	if (!g)
		return ;
	if (g->mlx)
	{
		free_image(g->mlx, &g->texture_no.imagen, &g->texture_no.addr);
		free_image(g->mlx, &g->texture_so.imagen, &g->texture_so.addr);
		free_image(g->mlx, &g->texture_we.imagen, &g->texture_we.addr);
		free_image(g->mlx, &g->texture_ea.imagen, &g->texture_ea.addr);
		free_image(g->mlx, &g->frame_img, &g->frame_addr);
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
/// 

int	check_char(char c, int flag_walkable)
{
	if (flag_walkable == 1)
		return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
	else
		return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E'
			|| c == 'W' || c == ' ');
}
///

/* int	check_map_closed(t_game *g)
{
	size_t	h;
	size_t	w;
	long long	total;
	int	py = -1;
	int	px = -1;
	size_t	i;
	size_t	j;
	char *visited = NULL;
	int *queue = NULL;
	size_t head = 0;
	size_t tail = 0;
	size_t start;
	long pos;
	int y;
	int x;
	char ch;
	char nch;
	size_t npos;

	if (!g || !g->map)
		return (write(2, "Error\nStruct\n", 13), -1);
	h = (size_t)g->map_height;
	w = (size_t)g->map_width;
	if (h == 0 || w == 0)
		return (write(2, "Error\nInvalid map size\n", 22), -1);
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < (size_t)ft_strlen(g->map[i]))
		{
			if (g->map[i][j] == 'N' || g->map[i][j] == 'S'
				|| g->map[i][j] == 'E' || g->map[i][j] == 'W')
			{
				py = (int)i;
				px = (int)j;
				break;
			}
			++j;
		}
		if (py != -1)
			break;
		++i;
	}
	if (py == -1)
		return (write(2, "Error\nPlayer not found\n", 23), -1);
	total = (long long)h * (long long)w;
	if (total <= 0)
		return (write(2, "Error\nInvalid map size\n", 22), -1);
	visited = ft_calloc((size_t)total, 1);
	if (!visited)
		return (perror("Error\nMalloc\n"), -1);
	queue = malloc(sizeof(int) * (size_t)total);
	if (!queue)
		return (free(visited), perror("Error\nMalloc\n"), -1);
	start = (size_t)py * w + (size_t)px;
	visited[start] = 1;
	queue[tail++] = (int)start;
	while (head < tail)
	{
		pos = queue[head++];
		y = (int)(pos / (long)w);
		x = (int)(pos % (long)w);
		ch = g->map[y][x];
		if (ch == ' ')
			return (free(queue), free(visited), write(2, "Error\nMap not closed\n", 21), -1);
		if ((size_t)(y + 1) < h)
		{
			npos = (size_t)(y + 1) * w + (size_t)x;
			if (!visited[npos])
			{
				nch = g->map[y + 1][x];
				if (nch != '1' && nch != ' ')
				{
					visited[npos] = 1;
					queue[tail++] = (int)npos;
				}
				else
				{
					visited[npos] = 1;
					if (nch == ' ')
						return (free(queue), free(visited), write(2, "Error\nMap not closed\n", 21), -1);
				}
			}
		}
		if (y - 1 >= 0)
		{
			npos = (size_t)(y - 1) * w + (size_t)x;
			if (!visited[npos])
			{
				nch = g->map[y - 1][x];
				if (nch != '1' && nch != ' ')
				{
					visited[npos] = 1;
					queue[tail++] = (int)npos;
				}
				else
				{
					visited[npos] = 1;
					if (nch == ' ')
						return (free(queue), free(visited), write(2, "Error\nMap not closed\n", 21), -1);
				}
			}
		}
		if ((size_t)(x + 1) < w)
		{
			npos = (size_t)y * w + (size_t)(x + 1);
			if (!visited[npos])
			{
				nch = g->map[y][x + 1];
				if (nch != '1' && nch != ' ')
				{
					visited[npos] = 1;
					queue[tail++] = (int)npos;
				}
				else
				{
					visited[npos] = 1;
					if (nch == ' ')
						return (free(queue), free(visited), write(2, "Error\nMap not closed\n", 21), -1);
				}
			}
		}
		if (x - 1 >= 0)
		{
			npos = (size_t)y * w + (size_t)(x - 1);
			if (!visited[npos])
			{
				nch = g->map[y][x - 1];
				if (nch != '1' && nch != ' ')
				{
					visited[npos] = 1;
					queue[tail++] = (int)npos;
				}
				else
				{
					visited[npos] = 1;
					if (nch == ' ')
						return (free(queue), free(visited), write(2, "Error\nMap not closed\n", 21), -1);
				}
			}
		}
	}
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			ch = g->map[i][j];
			if (ch == '0' || ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
			{
				if (!visited[i * w + j])
					return (free(queue), free(visited), write(2, "Error\nArea\n", 11), -1);
			}
			++j;
		}
		++i;
	}
	return (free(queue), free(visited), 0);
} */

int	find_player_pos(t_game *g, int *out_py, int *out_px)
{
	size_t	i;
	size_t	j;

	if (!g || !g->map || !out_py || !out_px)
		return (-1);
	*out_py = -1;
	*out_px = -1;
	i = 0;
	while (i < (size_t)g->map_height)
	{
		j = 0;
		while (j < (size_t)ft_strlen(g->map[i]))
		{
			if (g->map[i][j] == 'N' || g->map[i][j] == 'S'
				|| g->map[i][j] == 'E' || g->map[i][j] == 'W')
			{
				*out_py = (int)i;
				*out_px = (int)j;
				return (0);
			}
			++j;
		}
		++i;
	}
	return (-1);
}

int	allocate_visited_and_queue(long long total, char **visited_out,
						int **queue_out)
{
	*visited_out = NULL;
	*queue_out = NULL;
	if (total <= 0)
		return (-1);
	*visited_out = ft_calloc((size_t)total, 1);
	if (!*visited_out)
		return (perror("Error\nMalloc\n"), -1);
	*queue_out = malloc(sizeof(int) * (size_t)total);
	if (!*queue_out)
		return (free(*visited_out), perror("Error\nMalloc\n"), -1);
	return (0);
}

int	process_neighbor(t_game *g, char *visited, int *queue, size_t *tail)
{
	int	ny;
	int	nx;

	ny = (int)(g->npos / g->map_width);
	nx = (int)(g->npos % g->map_width);
	if (visited[g->npos])
		return (0);
	visited[g->npos] = 1;
	if (g->map[ny][nx] == ' ')
		return (write(2, "Error\nMap not closed\n", 21), -1);
	if (g->map[ny][nx] != '1')
	{
		queue[(*tail)++] = (int)g->npos;
	}
	return (0);
}

int	bfs_from_start(t_game *g, char *visited, int *queue, size_t start)
{
	long	pos;
	size_t	head;
	size_t	tail;
	int		y;
	int		x;

	tail = 0;
	head = 0;
	visited[start] = 1;
	queue[tail++] = (int)start;
	while (head < tail)
	{
		pos = queue[head++];
		y = (int)(pos / (long)g->map_width);
		x = (int)(pos % (long)g->map_width);
		if (g->map[y][x] == ' ')
			return (write(2, "Error\nMap not closed\n", 21), -1);
		if ((size_t)(y + 1) < (size_t)g->map_height)
		{
			g->npos = (size_t)(y + 1) * g->map_width + (size_t)x;
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
		if (y > 0)
		{
			g->npos = (size_t)(y - 1) * g->map_width + (size_t)x;
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
		if ((size_t)(x + 1) < (size_t)g->map_width)
		{
			g->npos = (size_t)y * g->map_width + (size_t)(x + 1);
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
		if (x > 0)
		{
			g->npos = (size_t)y * g->map_width + (size_t)(x - 1);
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
	}
	return (0);
}

int	check_all_visited(t_game *g, char *visited, size_t w, size_t h)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if (check_char(g->map[i][j], 1))
			{
				if (!visited[i * w + j])
					return (write(2, "Error\nArea\n", 11), -1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

int	check_map_closed(t_game *g)
{
	long long	total;
	char		*visited;
	int			*queue;
	int			px;
	int			py;

	visited = NULL;
	queue = NULL;
	if (!g || !g->map)
		return (write(2, "Error\nStruct\n", 13), -1);
	if (g->map_height == 0 || g->map_width == 0)
		return (write(2, "Error\nInvalid map size\n", 22), -1);
	if (find_player_pos(g, &py, &px) < 0)
		return (write(2, "Error\nPlayer not found\n", 23), -1);
	total = (long long)g->map_height * (long long)g->map_width;
	if (allocate_visited_and_queue(total, &visited, &queue) < 0)
		return (-1);
	if (bfs_from_start(g, visited, queue,
			((size_t)py * g->map_width + (size_t)px)) < 0)
		return (free(queue), free(visited), -1);
	if (check_all_visited(g, visited, g->map_width, g->map_height) < 0)
		return (free(queue), free(visited), -1);
	return (free(queue), free(visited), 0);
}

/*-----------------------------------------------------------------*/

/* int	parse_map(int fd, char *first_line, t_game *g)
{
	char	**lines = NULL;
	char	*line;
	size_t	cap = 0;
	size_t	count = 0;
	size_t	i;
	size_t	newcap;
	char	**tmp;

	if (!first_line || !g)
		return (perror("Error\nStruct\n"), -1);
	cap = 64;
	lines = malloc(sizeof(char *) * cap);
	if (!lines)
		return (free(first_line), perror("Error\nMalloc\n"), -1);
	lines[count++] = first_line;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (count >= cap)
		{
			newcap = cap * 2;
			tmp = malloc(sizeof(char *) * newcap);
			if (!tmp)
			{
				free(line);
				while (count--)
					free(lines[count]);
				return (free(lines), perror("Error\nMalloc\n"), -1);
			}
			i = 0;
			while (i < count)
			{
				tmp[i] = lines[i];
				i++;
			}
			free(lines);
			lines = tmp;
			cap = newcap;
		}
		lines[count++] = line;
		line = get_next_line(fd);
	}
	g->map_height = (int)count;
	g->map = malloc(sizeof(char *) * (g->map_height + 1));
	if (!g->map)
	{
		while (count--)
			free(lines[count]);
		return (free(lines), perror("Error\nMalloc\n"), -1);
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
} */

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

/*--------------------------------------------------------------------------*/
//

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
//

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

int	parse_color_value(const char *rest, int out_rgb[3])
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
//

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

void	strip_nl(char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
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

int	get_header_index(const char *trim)
{
	if (ft_strncmp(trim, "NO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (0);
	if (ft_strncmp(trim, "SO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (1);
	if (ft_strncmp(trim, "WE", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (2);
	if (ft_strncmp(trim, "EA", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (3);
	if (trim[0] == 'F' && (trim[1] == ' ' || trim[1] == '\t'))
		return (4);
	if (trim[0] == 'C' && (trim[1] == ' ' || trim[1] == '\t'))
		return (5);
	return (-1);
}

int	check_order(int *order, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
	{
		if (!order[i])
			return (-1);
		i++;
	}
	return (0);
}

int	apply_header_action(int idx, t_game *g, char *trim)
{
	if ((idx == 0 && (g->has_no || set_texture(&g->no, trim + 2) < 0))
		|| (idx == 1 && (g->has_so || set_texture(&g->so, trim + 2) < 0))
		|| (idx == 2 && (g->has_we || set_texture(&g->we, trim + 2) < 0))
		|| (idx == 3 && (g->has_ea || set_texture(&g->ea, trim + 2) < 0))
		|| (idx == 4 && (g->has_f || parse_color_value(trim + 1, g->floor) < 0))
		|| (idx == 5 && (g->has_c
				|| parse_color_value(trim + 1, g->ceiling) < 0)))
		return (-1);
	if (idx == 0)
		g->has_no = 1;
	else if (idx == 1)
		g->has_so = 1;
	else if (idx == 2)
		g->has_we = 1;
	else if (idx == 3)
		g->has_ea = 1;
	else if (idx == 4)
		g->has_f = 1;
	else if (idx == 5)
		g->has_c = 1;
	return (0);
}

int	parse_one_header_ordered(const char *line, t_game *g, int *order)
{
	char	*trim;
	int		idx;

	if (!line || !g || !order)
		return (-1);
	trim = ft_strtrim(line, " \t\n\r");
	if (!trim)
		return (-1);
	idx = get_header_index(trim);
	if (idx == -1)
		return (free(trim), -1);
	if (check_order(order, idx) < 0)
		return (free(trim), -1);
	if (apply_header_action(idx, g, trim) < 0)
		return (free(trim), -1);
	order[idx] = 1;
	return (free(trim), 0);
}

int	parse_headers_loop(int fd, t_game *g, char **out_first_map_line, int order[6])
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		if (is_blank_line(line))
		{
			(free(line), line = get_next_line(fd));
			continue ;
		}
		if (is_header_line(line))
		{
			if (parse_one_header_ordered(line, g, order) < 0)
				return (free(line), -1);
			(free(line), line = get_next_line(fd));
			continue ;
		}
		if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea
			|| !g->has_f || !g->has_c)
			return (free(line), -1);
		return (*out_first_map_line = line, 0);
	}
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
//

void	move_forward_backward(t_game *g, double move_speed, int direction)
{
	double	nx;
	double	ny;

	nx = g->posx + g->dirx * move_speed * direction;
	ny = g->posy + g->diry * move_speed * direction;
	if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0
		&& (int)g->posx < g->map_width && g->map[(int)ny][(int)g->posx] != '1'
		&& g->map[(int)ny][(int)g->posx] != ' ')
		g->posy = ny;
	if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0
		&& nx < g->map_width && g->map[(int)g->posy][(int)nx] != '1'
		&& g->map[(int)g->posy][(int)nx] != ' ')
		g->posx = nx;
}

void	move_left_right(t_game *g, double move_speed, int direction)
{
	double	nx;
	double	ny;

	nx = g->posx + g->planex * move_speed * direction;
	ny = g->posy + g->planey * move_speed * direction;
	if (ny >= 0 && ny < g->map_height && (int)g->posx < g->map_width
		&& (int)g->posx >= 0 && g->map[(int)ny][(int)g->posx] != '1'
		&& g->map[(int)ny][(int)g->posx] != ' ')
		g->posy = ny;
	if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0
		&& nx < g->map_width && g->map[(int)g->posy][(int)nx] != '1'
		&& g->map[(int)g->posy][(int)nx] != ' ')
		g->posx = nx;
}

void	rotate_player(t_game *g, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = g->dirx;
	g->dirx = g->dirx * cos(angle) - g->diry * sin(angle);
	g->diry = old_dir_x * sin(angle) + g->diry * cos(angle);
	old_plane_x = g->planex;
	g->planex = g->planex * cos(angle) - g->planey * sin(angle);
	g->planey = old_plane_x * sin(angle) + g->planey * cos(angle);
}

void	update_player(t_game *g, double delta)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 3.0 * delta;
	rot_speed = 2.0 * delta;
	if (g->key_w)
		move_forward_backward(g, move_speed, +1);
	if (g->key_s)
		move_forward_backward(g, move_speed, -1);
	if (g->key_a)
		move_left_right(g, move_speed, -1);
	if (g->key_d)
		move_left_right(g, move_speed, +1);
	if (g->key_left)
		rotate_player(g, -rot_speed);
	if (g->key_right)
		rotate_player(g, rot_speed);
}
//

int	get_tex_pixel(t_texture *texture, int x, int y)
{
	char	*px;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0);
	px = texture->addr + (y * texture->line_len_byte + x
			* (texture->bit_by_pixel / 8));
	return (*(unsigned int *)px);
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
	r->tex_pos = (r->draw_start - g->screen_h / 2 + r->line_height / 2)
		* r->step;
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

//

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
		r->wall_dist = (r->map_x - g->posx + (1 - r->step_x) / 2.0) / r->denom;
	}
	else
	{
		if (r->ray_dir_y == 0)
			r->denom = 1e-6;
		else
			r->denom = r->ray_dir_y;
		r->wall_dist = (r->map_y - g->posy + (1 - r->step_y) / 2.0) / r->denom;
	}
	if (r->wall_dist <= 0.0)
		r->wall_dist = 1e-6;
	r->line_height = (int)(g->screen_h / r->wall_dist);
	r->draw_start = -r->line_height / 2 + g->screen_h / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	r->draw_end = r->line_height / 2 + g->screen_h / 2;
	if (r->draw_end >= g->screen_h)
		r->draw_end = g->screen_h - 1;
}

t_texture	*choose_wall_texture(t_game *g, t_render *r)
{
	if (!g || !r)
		return (NULL);
	if (r->side == 0)
	{
		if (r->ray_dir_x > 0)
			return (&g->texture_we);
		else
			return (&g->texture_ea);
	}
	else
	{
		if (r->ray_dir_y > 0)
			return (&g->texture_no);
		else
			return (&g->texture_so);
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
			r->wall_x = g->posy + r->wall_dist * r->ray_dir_y;
		else
			r->wall_x = g->posx + r->wall_dist * r->ray_dir_x;
		r->wall_x -= floor(r->wall_x);
		*tex = choose_wall_texture(g, r);
		if (!(*tex) || !(*tex)->addr || (*tex)->width <= 0)
		{
			r->x++;
			continue ;
		}
		r->tex_width = (*tex)->width;
		r->tex_height = (*tex)->height;
		render_column(g, r, *tex);
		r->x++;
	}
}
//

double	get_time_s(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
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

int	game_loop(void *param)
{
	static double	last;
	double			delta;
	t_game			*g;
	t_render		r;
	t_texture		*tex;

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
//

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

int	check_file_empty(const char *path)
{
	int		fd;
	int		result;
	char	tmp[1];

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror("Error\nOpen"), -1);
	result = read(fd, tmp, 1);
	if (result < 0)
	{
		close(fd);
		return (perror("Error\nRead"), -1);
	}
	if (result == 0)
	{
		close(fd);
		return (write(1, "Error\nEmpty file\n", 17), -1);
	}
	close(fd);
	return (0);
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

int	find_player(t_game *g, char *out_orient)
{
	int	y;
	int	x;

	if (!g || !g->map || !out_orient)
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
				g->init_player_y = y;
				g->init_player_x = x;
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

int	parse(char **argv, t_game *g, char *orient)
{
	int		fd;
	char	*first_map_line;

	first_map_line = NULL;
	if (!extension_is_cub(argv[1]))
		return (write(2, "Error\nExtension\n", 16), -1);
	if (check_file_empty(argv[1]) < 0)
		return (-1);
	fd = open(argv[1], O_RDONLY);
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
	if (check_empty_lines_in_map(g) < 0 || pad_map(g, 0) < 0
		|| validate_map_chars(g) < 0 || check_map_closed(g) < 0
		|| find_player(g, orient) < 0)
		return (free_map(g), free_config(g), close(fd), -1);
	return (close(fd), 0);
}
//

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

void	init_player(t_game *g, char orient)
{
	g->dirx = 0.0;
	g->diry = 0.0;
	g->planex = 0.0;
	g->planey = 0.0;
	if (orient == 'N')
	{
		g->diry = -1.0;
		g->planex = 0.66;
	}
	else if (orient == 'S')
	{
		g->diry = 1.0;
		g->planex = -0.66;
	}
	else if (orient == 'E')
	{
		g->dirx = 1.0;
		g->planey = 0.66;
	}
	else if (orient == 'W')
	{
		g->dirx = -1.0;
		g->planey = -0.66;
	}
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

int	main(int argc, char **argv)
{
	t_game		g;
	char		orient;

	if (argc != 2)
		return (write(2, "Error\nArguments\n", 16), -1);
	if (parse(argv, &g, &orient) < 0)
		return (1);
	if (load_texture(g.mlx, &g.texture_no, g.no) < 0 || load_texture(g.mlx,
			&g.texture_so, g.so) < 0 || load_texture(g.mlx, &g.texture_we,
			g.we) < 0 || load_texture(g.mlx, &g.texture_ea, g.ea) < 0)
		return (free_map(&g), free_config(&g), 1);
	g.window = mlx_new_window(g.mlx, g.screen_w, g.screen_h, "cub3D");
	if (!g.window)
		return (perror("Error\nWindow\n"), free_map(&g), free_config(&g), 1);
	g.posx = (double)g.init_player_x + 0.5;
	g.posy = (double)g.init_player_y + 0.5;
	init_player(&g, orient);
	if (create_frame(&g) < 0)
		return (free_map(&g), free_config(&g), 1);
	mlx_hook(g.window, 2, 1, key_press, &g);
	mlx_hook(g.window, 3, 2, key_release, &g);
	mlx_hook(g.window, 17, 0, handle_close, &g);
	mlx_hook(g.window, 6, 64, mouse_move, &g);
	(mlx_loop_hook(g.mlx, game_loop, &g), mlx_loop(g.mlx));
	return (free_map(&g), free_config(&g), 0);
}
