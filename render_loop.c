/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:57:02 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/10 18:25:40 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	get_current_time_s(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec + time.tv_usec / 1000000.0);
}

int	create_frame_image(t_game *g)
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

void	draw_frame_background(t_game *g)
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

void	render_frame(t_game *g, t_render *r, t_texture **tex)
{
	if (!g || !g->frame_img || !g->frame_addr)
		return ;
	r->x = 0;
	while (r->x < g->screen_w)
	{
		init_render_vars(g, r, r->x);
		init_ray_steps(g, r);
		cast_ray(g, r);
		calculate_wall_projection(g, r);
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

int	game_loop(void *param)
{
	static double	last_time;
	double			delta_time;
	t_game			*g;
	t_render		r;
	t_texture		*tex;

	g = (t_game *)param;
	g->now = get_current_time_s();
	if (!last_time)
		last_time = g->now;
	delta_time = g->now - last_time;
	if (delta_time > 0.25)
		delta_time = 0.25;
	last_time = g->now;
	update_player(g, delta_time);
	if (!g->frame_img)
		if (create_frame_image(g) < 0)
			return (write(2, "Error\ncreate_frame failed\n", 26), 1);
	draw_frame_background(g);
	render_frame(g, &r, &tex);
	mlx_put_image_to_window(g->mlx, g->window, g->frame_img, 0, 0);
	return (0);
}
