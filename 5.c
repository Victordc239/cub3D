/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:53:40 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/02 16:54:05 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
