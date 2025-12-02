/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:49:37 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/02 17:32:15 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_header_index(const char *trim)
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

int	check_order(int *order, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
	{
		if (!order[i])
			return (-1);
		i++;
	}
	return (0);
}

int	parse_one_header_ordered(const char *line, t_game *g, int *order)
{
	char	*trim;
	int		idx;

	if (!line || !g || !order)
		return (-1);
	trim = ft_strtrim(line, " \t\n\r");
	if (!trim)
		return (-1);
	idx = get_header_index(trim);
	if (idx == -1)
		return (free(trim), -1);
	if (check_order(order, idx) < 0)
		return (free(trim), -1);
	if (apply_header_action(idx, g, trim) < 0)
		return (free(trim), -1);
	order[idx] = 1;
	return (free(trim), 0);
}
