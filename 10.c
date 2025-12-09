/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:40:34 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/09 14:40:17 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	find_player_pos(t_game *g, int *out_py, int *out_px)
{
	size_t	i;
	size_t	j;

	if (!g || !g->map || !out_py || !out_px)
		return (-1);
	*out_py = -1;
	*out_px = -1;
	i = 0;
	while (i < (size_t)g->map_height)
	{
		j = 0;
		while (j < (size_t)ft_strlen(g->map[i]))
		{
			if (g->map[i][j] == 'N' || g->map[i][j] == 'S'
				|| g->map[i][j] == 'E' || g->map[i][j] == 'W')
			{
				*out_py = (int)i;
				*out_px = (int)j;
				return (0);
			}
			++j;
		}
		++i;
	}
	return (-1);
}

int	allocate_visited_and_queue(long long total, char **visited_out,
						int **queue_out)
{
	*visited_out = NULL;
	*queue_out = NULL;
	if (total <= 0)
		return (-1);
	*visited_out = ft_calloc((size_t)total, 1);
	if (!*visited_out)
		return (perror("Error\nMalloc\n"), -1);
	*queue_out = malloc(sizeof(int) * (size_t)total);
	if (!*queue_out)
		return (free(*visited_out), perror("Error\nMalloc\n"), -1);
	return (0);
}

int	check_all_visited(t_game *g, char *visited, size_t w, size_t h)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if (check_char(g->map[i][j], 1))
			{
				if (!visited[i * w + j])
					return (write(2, "Error\nArea\n", 11), -1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

int	check_map_closed(t_game *g)
{
	long long	total;
	char		*visited;
	int			*queue;
	int			px;
	int			py;

	visited = NULL;
	queue = NULL;
	if (!g || !g->map)
		return (write(2, "Error\nStruct\n", 13), -1);
	if (g->map_height == 0 || g->map_width == 0)
		return (write(2, "Error\nInvalid map size\n", 22), -1);
	if (find_player_pos(g, &py, &px) < 0)
		return (write(2, "Error\nPlayer not found\n", 23), -1);
	total = (long long)g->map_height * (long long)g->map_width;
	if (allocate_visited_and_queue(total, &visited, &queue) < 0)
		return (-1);
	if (bfs_from_start(g, visited, queue,
			((size_t)py * g->map_width + (size_t)px)) < 0)
		return (free(queue), free(visited), -1);
	if (check_all_visited(g, visited, g->map_width, g->map_height) < 0)
		return (free(queue), free(visited), -1);
	return (free(queue), free(visited), 0);
}
