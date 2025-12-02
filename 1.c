/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:35:02 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/02 17:23:50 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
