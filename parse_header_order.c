/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_order.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:49:37 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/15 13:28:01 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_header_id(const char *header_line)
{
	if (ft_strncmp(header_line, "NO", 2) == 0
		&& (header_line[2] == ' ' || header_line[2] == '\t'))
		return (0);
	if (ft_strncmp(header_line, "SO", 2) == 0
		&& (header_line[2] == ' ' || header_line[2] == '\t'))
		return (1);
	if (ft_strncmp(header_line, "WE", 2) == 0
		&& (header_line[2] == ' ' || header_line[2] == '\t'))
		return (2);
	if (ft_strncmp(header_line, "EA", 2) == 0
		&& (header_line[2] == ' ' || header_line[2] == '\t'))
		return (3);
	if (header_line[0] == 'F'
		&& (header_line[1] == ' ' || header_line[1] == '\t'))
		return (4);
	if (header_line[0] == 'C'
		&& (header_line[1] == ' ' || header_line[1] == '\t'))
		return (5);
	return (-1);
}

int	check_header_order(int *order, int id)
{
	int	i;

	i = 0;
	while (i < id)
	{
		if (!order[i])
			return (-1);
		i++;
	}
	return (0);
}

int	parse_header_order(const char *line, t_game *g, int *order)
{
	char	*header_line;
	int		id;

	if (!line || !g || !order)
		return (-1);
	header_line = ft_strtrim(line, " \t\n\r");
	if (!header_line)
		return (-1);
	id = get_header_id(header_line);
	if (id == -1)
		return (free(header_line), -1);
	if (check_header_order(order, id) < 0)
		return (free(header_line), -1);
	if (process_header(id, g, header_line) < 0)
		return (free(header_line), -1);
	order[id] = 1;
	return (free(header_line), 0);
}
