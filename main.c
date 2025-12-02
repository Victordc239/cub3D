/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:57 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/12/02 17:49:53 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
