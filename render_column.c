/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:54:58 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/15 14:18:55 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_texture_pixel(t_texture *texture, int col_x, int row_y)
{
	char	*pixel;

	if (col_x < 0 || col_x >= texture->width || row_y < 0
		|| row_y >= texture->height)
		return (0);
	pixel = texture->addr + (row_y * texture->line_len_byte + col_x
			* (texture->bit_by_pixel / 8));
	return (*(unsigned int *)pixel);
}

void	draw_pixel_frame(t_game *g, int col_x, int row_y, unsigned int color)
{
	char	*frame_pixel;

	if (!g || !g->frame_addr)
		return ;
	if (col_x < 0 || col_x >= g->screen_w || row_y < 0 || row_y >= g->screen_h)
		return ;
	frame_pixel = g->frame_addr + row_y * g->frame_line_len
		+ col_x * (g->frame_bpp / 8);
	*(unsigned int *)frame_pixel = color;
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
		r->color = get_texture_pixel(tex, r->tex_x, r->tex_y);
		if (r->side == 1)
			r->color = ((r->color & 0xFEFEFE) >> 1);
		draw_pixel_frame(g, r->col_x, r->draw_start, r->color);
		r->draw_start++;
	}
}
