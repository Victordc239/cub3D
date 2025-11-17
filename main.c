/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:57 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/11/17 18:07:09 by vdiez-cu         ###   ########.fr       */
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
			if (parse_one_header(line, g) < 0)
				return (free(line), free_config(g), -1);
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		*out_first_map_line = line;
		return (0);
	}
	if (!g->has_no || !g->has_so || !g->has_we || !g->has_ea || !g->has_floor
		|| !g->has_ceiling)
		return (free_config(g), -1);
	return (0);
}

int	pad_map(t_game *g)
{
	int		y;
	int		len;
	int		i;
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

int	handle_key(int keycode, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	(void)g;
	if (keycode == 65307)
	{
		if (g->window)
			mlx_destroy_window(g->mlx, g->window);
		free_map(g);
		free_config(g);
		exit(0);
	}
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
		return (-1);
	texture->addr = mlx_get_data_addr(texture->imagen, &texture->bit_by_pixel,
			&texture->line_len_byte, &texture->endian);
	return (0);
}

t_texture	*get_texture_from_char(t_game *g, char c)
{
	if (c == 'N')
		return (&g->texture_no);
	if (c == 'S')
		return (&g->texture_so);
	if (c == 'W')
		return (&g->texture_we);
	if (c == 'E')
		return (&g->texture_ea);
	return (NULL);
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

void	draw_background(t_game *g, int w, int h)
{
	void	*img;
	char	*data;
	int		bpp;
	int		line_len;
	int		endian;
	int		x;
	int		y;
	unsigned int	color;

	img = mlx_new_image(g->mlx, w, h);
	if (!img)
		return ;
	data = mlx_get_data_addr(img, &bpp, &line_len, &endian);
	if (!data)
	{
		mlx_destroy_image(g->mlx, img);
		return ;
	}
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			if (y < h / 2)
				color = (g->ceiling[0] << 16) | (g->ceiling[1] << 8) | g->ceiling[2];
			else
				color = (g->floor[0] << 16) | (g->floor[1] << 8) | g->floor[2];
			*(unsigned int *)(data + (y * line_len + x * (bpp / 8))) = color;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(g->mlx, g->window, img, 0, 0);
	mlx_destroy_image(g->mlx, img);
}

void	blit_texture_at(t_game *g, t_texture *tex, int dst_x, int dst_y)
{
	void		*img;
	char	*data;
	int		bpp;
	int		line;
	int		end;
	int		x;
	int		y;
	unsigned int	color;

	if (!tex || !tex->imagen)
		return ;
	img = mlx_new_image(g->mlx, tex->width, tex->height);
	if (!img)
		return ;
	data = mlx_get_data_addr(img, &bpp, &line, &end);
	y = 0;
	while (y < tex->height)
	{
		x = 0;
		while (x < tex->width)
		{
			color = *(unsigned int *)(tex->addr + y * tex->line_len_byte
					+ x * (tex->bit_by_pixel / 8));
			*(unsigned int *)(data + y * line + x * (bpp / 8)) = color;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(g->mlx, g->window, img, dst_x, dst_y);
	mlx_destroy_image(g->mlx, img);
}

//---------------------------------------------------------------------------------

#include <math.h>
#include <sys/time.h>

/* pinta techo y suelo directamente en g->frame_addr (frame_img debe existir) */
void draw_background_to_frame(t_game *g)
{
    int x, y;
    unsigned int color_ceiling;
    unsigned int color_floor;
    char *data;
    int bpp;
    int line_len;
    int endian;

    if (!g || !g->frame_img || !g->frame_addr)
        return;

    /* colores RGB -> 0xRRGGBB */
    color_ceiling = (g->ceiling[0] << 16) | (g->ceiling[1] << 8) | g->ceiling[2];
    color_floor   = (g->floor[0]   << 16) | (g->floor[1]   << 8) | g->floor[2];

    data = g->frame_addr;
    bpp = g->frame_bpp;
    line_len = g->frame_line_len;
    endian = g->frame_endian; /* no usado pero por claridad */

    y = 0;
    while (y < g->screen_h)
    {
        x = 0;
        while (x < g->screen_w)
        {
            if (y < g->screen_h / 2)
                *(unsigned int *)(data + y * line_len + x * (bpp / 8)) = color_ceiling;
            else
                *(unsigned int *)(data + y * line_len + x * (bpp / 8)) = color_floor;
            x++;
        }
        y++;
    }
}


/* inicializa jugador según la orientación encontrada */
void init_player(t_game *g, int py, int px, char orient)
{
    g->posx = (double)px + 0.5;
    g->posy = (double)py + 0.5;
    if (orient == 'N')
    {
        g->dirx = 0.0; g->diry = -1.0;
        g->planex = 0.66; g->planey = 0.0;
    }
    else if (orient == 'S')
    {
        g->dirx = 0.0; g->diry = 1.0;
        g->planex = -0.66; g->planey = 0.0;
    }
    else if (orient == 'E')
    {
        g->dirx = 1.0; g->diry = 0.0;
        g->planex = 0.0; g->planey = 0.66;
    }
    else if (orient == 'W')
    {
        g->dirx = -1.0; g->diry = 0.0;
        g->planex = 0.0; g->planey = -0.66;
    }
    g->key_w = g->key_s = g->key_a = g->key_d = g->key_left = g->key_right = 0;
}

/* crear / recrear imagen de frame */
int create_frame(t_game *g)
{
    if (g->frame_img)
        mlx_destroy_image(g->mlx, g->frame_img);
    g->frame_img = mlx_new_image(g->mlx, g->screen_w, g->screen_h);
    if (!g->frame_img)
        return (-1);
    g->frame_addr = mlx_get_data_addr(g->frame_img,
            &g->frame_bpp, &g->frame_line_len, &g->frame_endian);
    return (0);
}

/* obtén tiempo en segundos */
double get_time_s(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/* handlers de teclado (presionar / soltar) */
int key_press(int keycode, void *param)
{
    t_game *g = (t_game *)param;
    if (keycode == 65307) /* ESC */
    {
        if (g->window)
            mlx_destroy_window(g->mlx, g->window);
        free_map(g);
        free_config(g);
        exit(0);
    }
    else if (keycode == 119 || keycode == 'w') g->key_w = 1;
    else if (keycode == 115 || keycode == 's') g->key_s = 1;
    else if (keycode == 97  || keycode == 'a') g->key_a = 1;
    else if (keycode == 100 || keycode == 'd') g->key_d = 1;
    else if (keycode == 65361) g->key_left = 1;
    else if (keycode == 65363) g->key_right = 1;
    return (0);
}

int key_release(int keycode, void *param)
{
    t_game *g = (t_game *)param;
    if (keycode == 119 || keycode == 'w') g->key_w = 0;
    else if (keycode == 115 || keycode == 's') g->key_s = 0;
    else if (keycode == 97  || keycode == 'a') g->key_a = 0;
    else if (keycode == 100 || keycode == 'd') g->key_d = 0;
    else if (keycode == 65361) g->key_left = 0;
    else if (keycode == 65363) g->key_right = 0;
    return (0);
}

/* actualizar jugador según flags (colisión simple por celda) */
void update_player(t_game *g, double delta)
{
    double moveSpeed = 3.0 * delta;
    double rotSpeed  = 2.0 * delta;
    double nx, ny;

    if (g->key_w)
    {
        nx = g->posx + g->dirx * moveSpeed;
        ny = g->posy + g->diry * moveSpeed;
        if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
            && g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
            g->posy = ny;
        if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
            && g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
            g->posx = nx;
    }
    if (g->key_s)
    {
        nx = g->posx - g->dirx * moveSpeed;
        ny = g->posy - g->diry * moveSpeed;
        if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
            && g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
            g->posy = ny;
        if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
            && g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
            g->posx = nx;
    }
    if (g->key_a)
    {
        nx = g->posx - g->planex * moveSpeed;
        ny = g->posy - g->planey * moveSpeed;
        if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
            && g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
            g->posy = ny;
        if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
            && g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
            g->posx = nx;
    }
    if (g->key_d)
    {
        nx = g->posx + g->planex * moveSpeed;
        ny = g->posy + g->planey * moveSpeed;
        if (ny >= 0 && ny < g->map_height && (int)g->posx >= 0 && (int)g->posx < g->map_width
            && g->map[(int)ny][(int)g->posx] != '1' && g->map[(int)ny][(int)g->posx] != ' ')
            g->posy = ny;
        if ((int)g->posy >= 0 && (int)g->posy < g->map_height && nx >= 0 && nx < g->map_width
            && g->map[(int)g->posy][(int)nx] != '1' && g->map[(int)g->posy][(int)nx] != ' ')
            g->posx = nx;
    }
    if (g->key_left)
    {
        double oldDirX = g->dirx;
        g->dirx = g->dirx * cos(rotSpeed) - g->diry * sin(rotSpeed);
        g->diry = oldDirX * sin(rotSpeed) + g->diry * cos(rotSpeed);
        double oldPlaneX = g->planex;
        g->planex = g->planex * cos(rotSpeed) - g->planey * sin(rotSpeed);
        g->planey = oldPlaneX * sin(rotSpeed) + g->planey * cos(rotSpeed);
    }
    if (g->key_right)
    {
        double oldDirX = g->dirx;
        g->dirx = g->dirx * cos(-rotSpeed) - g->diry * sin(-rotSpeed);
        g->diry = oldDirX * sin(-rotSpeed) + g->diry * cos(-rotSpeed);
        double oldPlaneX = g->planex;
        g->planex = g->planex * cos(-rotSpeed) - g->planey * sin(-rotSpeed);
        g->planey = oldPlaneX * sin(-rotSpeed) + g->planey * cos(-rotSpeed);
    }
}


/* ============================
   RENDER: put_pixel_frame, choose_wall_texture, render_frame
   ============================ */

#include <math.h>

/* escribe un pixel en la imagen del frame (frame_img) */
void put_pixel_frame(t_game *g, int x, int y, unsigned int color)
{
	char *dst;

	if (!g || !g->frame_addr)
		return;
	if (x < 0 || x >= g->screen_w || y < 0 || y >= g->screen_h)
		return;
	dst = g->frame_addr + y * g->frame_line_len + x * (g->frame_bpp / 8);
	*(unsigned int *)dst = color;
}

/* elige la textura adecuada según side y dirección del rayo
   side == 0 -> golpe en pared vertical (x-side); side == 1 -> horizontal (y-side)
   Devolvemos puntero a la textura cargada en g. */
t_texture *choose_wall_texture(t_game *g, int side, double rayDirX, double rayDirY)
{
	if (!g)
		return (NULL);
	if (side == 0)
	{
		/* vertical wall: comparando signo de rayDirX */
		if (rayDirX > 0)
			return (&g->texture_we); /* rayo hacia +X, la cara que se ve es la W */
		else
			return (&g->texture_ea); /* rayo hacia -X, la cara que se ve es la E */
	}
	else
	{
		/* horizontal wall: comparar signo de rayDirY */
		if (rayDirY > 0)
			return (&g->texture_no); /* rayo hacia +Y, la cara que se ve es la N */
		else
			return (&g->texture_so); /* rayo hacia -Y, la cara que se ve es la S */
	}
}

/* Raycaster: pinta las paredes sobre g->frame_addr (asume fondo ya pintado).
   Usa get_tex_pixel para muestrear la textura. */
void render_frame(t_game *g)
{
	int x;

	if (!g || !g->frame_img || !g->frame_addr)
		return;

	for (x = 0; x < g->screen_w; x++)
	{
		/* calcular rayo por columna */
		double cameraX = 2.0 * x / (double)g->screen_w - 1.0;
		double rayDirX = g->dirx + g->planex * cameraX;
		double rayDirY = g->diry + g->planey * cameraX;

		int mapX = (int)g->posx;
		int mapY = (int)g->posy;

		double sideDistX;
		double sideDistY;

		double deltaDistX = (rayDirX == 0.0) ? 1e30 : fabs(1.0 / rayDirX);
		double deltaDistY = (rayDirY == 0.0) ? 1e30 : fabs(1.0 / rayDirY);
		int stepX;
		int stepY;
		int hit = 0;
		int side = 0;

		/* inicializar step y sideDist */
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (g->posx - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - g->posx) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (g->posy - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - g->posy) * deltaDistY;
		}

		/* DDA - buscar colisión con pared */
		while (!hit)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			/* fuera del mapa -> tratamos como hit para evitar bucle infinito */
			if (mapY < 0 || mapY >= g->map_height || mapX < 0 || mapX >= g->map_width)
			{
				hit = 1;
				break;
			}
			char mch = g->map[mapY][mapX];
			if (mch == '1')
				hit = 1;
			else if (mch == ' ')
			{
				/* espacio indica borde: tratar como hit para seguridad */
				hit = 1;
			}
		}

		/* distancia perpendicular al muro */
		double perpWallDist;
		if (side == 0)
			perpWallDist = (mapX - g->posx + (1 - stepX) / 2.0) / (rayDirX == 0 ? 1e-6 : rayDirX);
		else
			perpWallDist = (mapY - g->posy + (1 - stepY) / 2.0) / (rayDirY == 0 ? 1e-6 : rayDirY);
		if (perpWallDist <= 0.0)
			perpWallDist = 1e-6;

		/* altura de la línea a dibujar */
		int lineHeight = (int)(g->screen_h / perpWallDist);

		int drawStart = -lineHeight / 2 + g->screen_h / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + g->screen_h / 2;
		if (drawEnd >= g->screen_h) drawEnd = g->screen_h - 1;

		/* calcular coordenada exacta del impacto en la pared (wallX en [0,1]) */
		double wallX;
		if (side == 0)
			wallX = g->posy + perpWallDist * rayDirY;
		else
			wallX = g->posx + perpWallDist * rayDirX;
		wallX -= floor(wallX);
		t_texture *tex = choose_wall_texture(g, side, rayDirX, rayDirY);
		if (!tex || !tex->addr || tex->width <= 0 || tex->height <= 0)
			continue;

		int texWidth = tex->width;
		int texHeight = tex->height;

		/* calcular texX (coordenada X en la textura) */
		int texX = (int)(wallX * (double)texWidth);
		/* invertir si hace falta (para que la orientación quede bien) */
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
			texX = texWidth - texX - 1;
		if (texX < 0) texX = 0;
		if (texX >= texWidth) texX = texWidth - 1;

		/* step y texPos para muestrear verticalmente */
		double step = 1.0 * texHeight / (double)lineHeight;
		double texPos = (drawStart - g->screen_h / 2 + lineHeight / 2) * step;

		/* dibujar columna (solo paredes; techo/suelo ya pintados) */
		int y;
		for (y = drawStart; y <= drawEnd; y++)
		{
			int texY = (int)texPos;
			if (texY < 0) texY = 0;
			if (texY >= texHeight) texY = texHeight - 1;
			texPos += step;
			unsigned int color = (unsigned int)get_tex_pixel(tex, texX, texY);
			/* oscurecer si side == 1 para dar sensación de profundidad */
			if (side == 1)
				color = ((color & 0xFEFEFE) >> 1); /* división aproximada por 2 */
			put_pixel_frame(g, x, y, color);
		}
	} /* fin columnas */
}

int game_loop(void *param)
{
	static double	last = 0.0;
	double			now;
	double			delta;
	t_game			*g;

	g = (t_game *)param;
	now = get_time_s();
	if (!last)
		last = now;
	delta = now - last;
	if (delta > 0.25)
		delta = 0.25;
	last = now;
	update_player(g, delta);
	if (!g->frame_img)
	{
		if (create_frame(g) < 0)
			return (write(2, "Error\ncreate_frame failed\n", 26), 1);
	}
	draw_background_to_frame(g);
	render_frame(g);
	mlx_put_image_to_window(g->mlx, g->window, g->frame_img, 0, 0);
	return (0);
}

//---------------------------------------------------------------------------------

/*int	main(int argc, char **argv)
{
	int		fd;
	t_game	g;
	char		*first_map_line;
	int		py;
	int		px;
	char		orient;

	if (argc != 2)
		return (write(2, "Error\nUsage: ./cub3D <file.cub>\n", 33), 1);
	if (!is_cub(argv[1]))
		return (write(2, "Error\nextension\n", 16), 1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Error\nopen"), 1);
	g.no = NULL;
	g.so = NULL;
	g.we = NULL;
	g.ea = NULL;
	g.has_no = 0;
	g.has_so = 0;
	g.has_we = 0;
	g.has_ea = 0;
	g.has_floor = 0;
	g.has_ceiling = 0;
	first_map_line = NULL;
	if (parse_headers(fd, &g, &first_map_line) < 0
		|| parse_map(fd, first_map_line, &g) < 0)
		return (close(fd), free_config(&g),
			write(2, "Error\nInvalid map\n", 18), 1);
	if (pad_map(&g) < 0)
		return (close(fd), free_map(&g), free_config(&g),
			write(2, "Error\nMalloc\n", 13), 1);
	if (validate_map_chars(&g) < 0 || check_map_closed(&g) < 0)
		return (free_map(&g), free_config(&g), 1);
	if (find_player(&g, &py, &px, &orient) < 0)
		return (write(2, "Error\nPlayer not found\n", 23),
			free_map(&g), free_config(&g), 1);
	close(fd);
	g.mlx = mlx_init();
	if (!g.mlx)
		return (write(2, "Error\nmlx_init failed\n", 22),
			free_map(&g), free_config(&g), 1);
	g.window = mlx_new_window(g.mlx, 3835, 2000, "cub3D");
	if (!g.window)
		return (write(2, "Error\nmlx_new_window failed\n", 28),
			free_map(&g), free_config(&g), 1);
	if (load_texture(g.mlx, &g.texture_no, g.no) < 0
		|| load_texture(g.mlx, &g.texture_so, g.so) < 0
		|| load_texture(g.mlx, &g.texture_we, g.we) < 0
		|| load_texture(g.mlx, &g.texture_ea, g.ea) < 0)
	{
		write(2, "Error\nTexture load failed\n", 26);
		free_map(&g);
		free_config(&g);
		return (1);
	}
	mlx_key_hook(g.window, handle_key, &g);
	mlx_hook(g.window, 17, 0, handle_close, &g);
	draw_background(&g, 3835, 2000);
	blit_texture_at(&g, &g.texture_no, 0, 0);
	mlx_loop(g.mlx);
	return (free_map(&g), free_config(&g), 0);
}*/
int	main(int argc, char **argv)
{
	int		fd;
	t_game	g;
	char	*first_map_line;
	int		py;
	int		px;
	char	orient;

	if (argc != 2)
		return (write(2, "Error\nUsage: ./cub3D <file.cub>\n", 33), 1);
	if (!is_cub(argv[1]))
		return (write(2, "Error\nextension\n", 16), 1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Error\nopen"), 1);

	/* inicializar campos de configuración */
	g.no = NULL;
	g.so = NULL;
	g.we = NULL;
	g.ea = NULL;
	g.has_no = 0;
	g.has_so = 0;
	g.has_we = 0;
	g.has_ea = 0;
	g.has_floor = 0;
	g.has_ceiling = 0;
	first_map_line = NULL;

	if (parse_headers(fd, &g, &first_map_line) < 0
		|| parse_map(fd, first_map_line, &g) < 0)
		return (close(fd), free_config(&g),
			write(2, "Error\nInvalid map\n", 18), 1);
	if (pad_map(&g) < 0)
		return (close(fd), free_map(&g), free_config(&g),
			write(2, "Error\nMalloc\n", 13), 1);
	if (validate_map_chars(&g) < 0 || check_map_closed(&g) < 0)
		return (free_map(&g), free_config(&g), 1);
	if (find_player(&g, &py, &px, &orient) < 0)
		return (write(2, "Error\nPlayer not found\n", 23),
			free_map(&g), free_config(&g), 1);
	close(fd);

	/* init mlx y ventana */
	g.mlx = mlx_init();
	if (!g.mlx)
		return (write(2, "Error\nmlx_init failed\n", 22),
			free_map(&g), free_config(&g), 1);

	/* tamaño de ventana (puedes ajustar) */
	g.screen_w = 2500;
	g.screen_h = 1700;
	g.window = mlx_new_window(g.mlx, g.screen_w, g.screen_h, "cub3D");
	if (!g.window)
		return (write(2, "Error\nmlx_new_window failed\n", 28),
			free_map(&g), free_config(&g), 1);

	/* cargar texturas */
	if (load_texture(g.mlx, &g.texture_no, g.no) < 0
		|| load_texture(g.mlx, &g.texture_so, g.so) < 0
		|| load_texture(g.mlx, &g.texture_we, g.we) < 0
		|| load_texture(g.mlx, &g.texture_ea, g.ea) < 0)
	{
		write(2, "Error\nTexture load failed\n", 26);
		free_map(&g);
		free_config(&g);
		return (1);
	}

	/* inicializar jugador/cámara con la posición encontrada */
	init_player(&g, py, px, orient);

	/* preparar frame buffer */
	g.frame_img = NULL;
	g.frame_addr = NULL;
	if (create_frame(&g) < 0)
	{
		write(2, "Error\nMalloc frame\n", 19);
		free_map(&g);
		free_config(&g);
		return (1);
	}

	/* registrar hooks */
	mlx_hook(g.window, 2, 1L << 0, key_press, &g);    /* KeyPress */
	mlx_hook(g.window, 3, 1L << 1, key_release, &g);  /* KeyRelease */
	mlx_hook(g.window, 17, 0, handle_close, &g);      /* Window close (X) */
	mlx_loop_hook(g.mlx, game_loop, &g);

	/* arrancar loop */
	mlx_loop(g.mlx);

	/* limpieza (normalmente no llegas aquí porque handle_close llama exit) */
	return (free_map(&g), free_config(&g), 0);
}
