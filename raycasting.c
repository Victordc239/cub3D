/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame(6).c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:55:45 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/10 17:47:59 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	init_ray_steps(t_game *g, t_render *r)
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

void	cast_ray(t_game *g, t_render *r)
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

void	calculate_wall_projection(t_game *g, t_render *r)
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
