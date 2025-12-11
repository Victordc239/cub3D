/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:04:35 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/11 18:12:52 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_empty_lines_map(t_game *g)
{
	int	row_y;
	int	map_content;

	if (!g || !g->map)
		return (-1);
	map_content = 0;
	row_y = 0;
	while (row_y < g->map_height)
	{
		if (!is_empty_line(g->map[row_y]))
			map_content = 1;
		else if (map_content)
		{
			write(2, "Error\nEmpty line in map\n", 25);
			return (-1);
		}
		row_y++;
	}
	return (0);
}

int	complete_map_rows(t_game *g, int row_y)
{
	int		row_len;
	int		col_x;
	char	*new_row;

	if (!g || !g->map)
		return (write(2, "Error\nStruct\n", 13), -1);
	row_y = -1;
	while (++row_y < g->map_height)
	{
		row_len = ft_strlen(g->map[row_y]);
		if (row_len < g->map_width)
		{
			new_row = malloc(g->map_width + 1);
			if (!new_row)
				return (write(2, "Error\nMalloc\n", 13), -1);
			col_x = -1;
			while (++col_x < row_len)
				new_row[col_x] = g->map[row_y][col_x];
			while (col_x < g->map_width)
				new_row[col_x++] = ' ';
			new_row[col_x] = '\0';
			(free(g->map[row_y]), g->map[row_y] = new_row);
		}
	}
	return (0);
}

int	is_valid_char(char c, int walkable_char)
{
	if (walkable_char == 1)
		return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
	else
		return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E'
			|| c == 'W' || c == ' ');
}

int	validate_map_chars(t_game *g)
{
	int	row_y;
	int	col_x;
	int	player_count;

	if (!g || !g->map)
		return (-1);
	player_count = 0;
	row_y = 0;
	while (g->map[row_y])
	{
		col_x = 0;
		while (g->map[row_y][col_x])
		{
			if (!is_valid_char(g->map[row_y][col_x], 0))
				return (write(2, "Error\nInvalid character in map\n", 31), -1);
			if (g->map[row_y][col_x] == 'N' || g->map[row_y][col_x] == 'S'
				|| g->map[row_y][col_x] == 'E' || g->map[row_y][col_x] == 'W')
				player_count++;
			col_x++;
		}
		row_y++;
	}
	if (player_count != 1)
		return (write(2, "Error\nInvalid player count\n", 27), -1);
	return (0);
}

int	init_player_pos(t_game *g, char *player_orientation)
{
	int	row_y;
	int	col_x;

	if (!g || !g->map || !player_orientation)
		return (write(2, "Error\nStruct\n", 13), -1);
	row_y = 0;
	while (row_y < g->map_height)
	{
		col_x = 0;
		while (g->map[row_y][col_x])
		{
			if (g->map[row_y][col_x] == 'N' || g->map[row_y][col_x] == 'S'
			|| g->map[row_y][col_x] == 'E' || g->map[row_y][col_x] == 'W')
			{
				g->init_player_y = row_y;
				g->init_player_x = col_x;
				*player_orientation = g->map[row_y][col_x];
				g->map[row_y][col_x] = '0';
				return (0);
			}
			col_x++;
		}
		row_y++;
	}
	return (write(2, "Error\nPlayer not found\n", 23), -1);
}
