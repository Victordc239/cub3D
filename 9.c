/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:39:46 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/10 16:24:39 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	process_neighbor(t_game *g, char *visited, int *queue, size_t *tail)
{
	int	next_y;
	int	next_x;

	next_y = (int)(g->npos / g->map_width);
	next_x = (int)(g->npos % g->map_width);
	if (visited[g->npos])
		return (0);
	visited[g->npos] = 1;
	if (g->map[next_y][next_x] == ' ')
		return (write(2, "Error\nMap not closed\n", 21), -1);
	if (g->map[next_y][next_x] != '1')
	{
		queue[(*tail)++] = (int)g->npos;
	}
	return (0);
}

int	process_neighbors_of(t_game *g, char *visited, int *queue, size_t *tail)
{
	if ((g->y_neighbor + 1) < (size_t)g->map_height)
	{
		g->npos = (g->y_neighbor + 1) * g->map_width + g->x_neighbor;
		if (process_neighbor(g, visited, queue, tail) < 0)
			return (-1);
	}
	if (g->y_neighbor > 0)
	{
		g->npos = (g->y_neighbor - 1) * g->map_width + g->x_neighbor;
		if (process_neighbor(g, visited, queue, tail) < 0)
			return (-1);
	}
	if ((g->x_neighbor + 1) < (size_t)g->map_width)
	{
		g->npos = g->y_neighbor * g->map_width + (g->x_neighbor + 1);
		if (process_neighbor(g, visited, queue, tail) < 0)
			return (-1);
	}
	if (g->x_neighbor > 0)
	{
		g->npos = g->y_neighbor * g->map_width + (g->x_neighbor - 1);
		if (process_neighbor(g, visited, queue, tail) < 0)
			return (-1);
	}
	return (0);
}

int	bfs_from_start(t_game *g, char *visited, int *queue, size_t start)
{
	long	pos;
	size_t	head;
	size_t	tail;

	tail = 0;
	head = 0;
	visited[start] = 1;
	queue[tail++] = (int)start;
	while (head < tail)
	{
		pos = queue[head++];
		g->y_neighbor = (int)(pos / (long)g->map_width);
		g->x_neighbor = (int)(pos % (long)g->map_width);
		if (g->map[g->y_neighbor][g->x_neighbor] == ' ')
			return (write(2, "Error\nMap not closed\n", 21), -1);
		if (process_neighbors_of(g, visited, queue, &tail) < 0)
			return (-1);
	}
	return (0);
}
