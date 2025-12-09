/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_order.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:49:37 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/09 17:49:03 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_header_id(const char *trim)
{
	if (ft_strncmp(trim, "NO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (0);
	if (ft_strncmp(trim, "SO", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (1);
	if (ft_strncmp(trim, "WE", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (2);
	if (ft_strncmp(trim, "EA", 2) == 0 && (trim[2] == ' ' || trim[2] == '\t'))
		return (3);
	if (trim[0] == 'F' && (trim[1] == ' ' || trim[1] == '\t'))
		return (4);
	if (trim[0] == 'C' && (trim[1] == ' ' || trim[1] == '\t'))
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
	char	*trim;
	int		id;

	if (!line || !g || !order)
		return (-1);
	trim = ft_strtrim(line, " \t\n\r");
	if (!trim)
		return (-1);
	id = get_header_id(trim);
	if (id == -1)
		return (free(trim), -1);
	if (check_header_order(order, id) < 0)
		return (free(trim), -1);
	if (process_header(id, g, trim) < 0)
		return (free(trim), -1);
	order[id] = 1;
	return (free(trim), 0);
}
