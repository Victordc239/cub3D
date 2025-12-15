/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_closed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:40:34 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/15 14:17:54 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	find_player_pos(t_game *g, int *player_y, int *player_x)
{
	size_t	row_y;
	size_t	col_x;

	if (!g || !g->map || !player_y || !player_x)
		return (-1);
	*player_y = -1;
	*player_x = -1;
	row_y = 0;
	while (row_y < (size_t)g->map_height)
	{
		col_x = 0;
		while (col_x < (size_t)ft_strlen(g->map[row_y]))
		{
			if (g->map[row_y][col_x] == 'N' || g->map[row_y][col_x] == 'S'
				|| g->map[row_y][col_x] == 'E' || g->map[row_y][col_x] == 'W')
			{
				*player_y = (int)row_y;
				*player_x = (int)col_x;
				return (0);
			}
			++col_x;
		}
		++row_y;
	}
	return (-1);
}

int	alloc_bfs_data(long long total_cells, char **visited_cells_out,
		int **pending_cells_out)
{
	*visited_cells_out = NULL;
	*pending_cells_out = NULL;
	if (total_cells <= 0)
		return (-1);
	*visited_cells_out = ft_calloc((size_t)total_cells, 1);
	if (!*visited_cells_out)
		return (perror("Error\nMalloc\n"), -1);
	*pending_cells_out = malloc(sizeof(int) * (size_t)total_cells);
	if (!*pending_cells_out)
		return (free(*visited_cells_out), perror("Error\nMalloc\n"), -1);
	return (0);
}

int	check_all_cells_visited(t_game *g, char *visited_cells, size_t width,
		size_t height)
{
	size_t	row_y;
	size_t	col_x;

	row_y = 0;
	while (row_y < height)
	{
		col_x = 0;
		while (col_x < width)
		{
			if (is_valid_char(g->map[row_y][col_x], 1))
			{
				if (!visited_cells[row_y * width + col_x])
					return (write(2, "Error\nArea\n", 11), -1);
			}
			++col_x;
		}
		++row_y;
	}
	return (0);
}

int	validate_map_closed(t_game *g)
{
	long long	total_cells;
	char		*visited_cells;
	int			*pending_cells;
	int			player_x;
	int			player_y;

	visited_cells = NULL;
	pending_cells = NULL;
	if (!g || !g->map)
		return (write(2, "Error\nStruct\n", 13), -1);
	if (g->map_height == 0 || g->map_width == 0)
		return (write(2, "Error\nInvalid map size\n", 22), -1);
	if (find_player_pos(g, &player_y, &player_x) < 0)
		return (write(2, "Error\nPlayer not found\n", 23), -1);
	total_cells = (long long)g->map_height * (long long)g->map_width;
	if (alloc_bfs_data(total_cells, &visited_cells, &pending_cells) < 0)
		return (-1);
	if (validate_map(g, visited_cells, pending_cells,
			((size_t)player_y * g->map_width + (size_t)player_x)) < 0)
		return (free(pending_cells), free(visited_cells), -1);
	if (check_all_cells_visited(g, visited_cells, g->map_width,
			g->map_height) < 0)
		return (free(pending_cells), free(visited_cells), -1);
	return (free(pending_cells), free(visited_cells), 0);
}
