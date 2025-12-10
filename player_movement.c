/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:53:40 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/10 17:13:35 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_forward_back(t_game *g, double move_speed, int direction)
{
	g->posx = g->posx + g->dirx * move_speed * direction;
	g->posy = g->posy + g->diry * move_speed * direction;
}

void	move_left_right(t_game *g, double move_speed, int direction)
{
	g->posx = g->posx + g->planex * move_speed * direction;
	g->posy = g->posy + g->planey * move_speed * direction;
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
