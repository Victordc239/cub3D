/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:04:35 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/02 17:40:32 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

int	check_char(char c, int flag_walkable)
{
	if (flag_walkable == 1)
		return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
	else
		return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E'
			|| c == 'W' || c == ' ');
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
