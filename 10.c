/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10a.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:39:46 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/09 14:40:28 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	process_neighbor(t_game *g, char *visited, int *queue, size_t *tail)
{
	int	ny;
	int	nx;

	ny = (int)(g->npos / g->map_width);
	nx = (int)(g->npos % g->map_width);
	if (visited[g->npos])
		return (0);
	visited[g->npos] = 1;
	if (g->map[ny][nx] == ' ')
		return (write(2, "Error\nMap not closed\n", 21), -1);
	if (g->map[ny][nx] != '1')
	{
		queue[(*tail)++] = (int)g->npos;
	}
	return (0);
}

int	bfs_from_start(t_game *g, char *visited, int *queue, size_t start)
{
	long	pos;
	size_t	head;
	size_t	tail;
	int		y;
	int		x;

	tail = 0;
	head = 0;
	visited[start] = 1;
	queue[tail++] = (int)start;
	while (head < tail)
	{
		pos = queue[head++];
		y = (int)(pos / (long)g->map_width);
		x = (int)(pos % (long)g->map_width);
		if (g->map[y][x] == ' ')
			return (write(2, "Error\nMap not closed\n", 21), -1);
		if ((size_t)(y + 1) < (size_t)g->map_height)
		{
			g->npos = (size_t)(y + 1) * g->map_width + (size_t)x;
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
		if (y > 0)
		{
			g->npos = (size_t)(y - 1) * g->map_width + (size_t)x;
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
		if ((size_t)(x + 1) < (size_t)g->map_width)
		{
			g->npos = (size_t)y * g->map_width + (size_t)(x + 1);
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
		if (x > 0)
		{
			g->npos = (size_t)y * g->map_width + (size_t)(x - 1);
			if (process_neighbor(g, visited, queue, &tail) < 0)
				return (-1);
		}
	}
	return (0);
}
