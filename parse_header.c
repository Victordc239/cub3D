/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:47:00 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/15 16:18:54 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_uint0_255(const char **rgb, int *rgb_number)
{
	long		value;
	const char	*colors_line;

	value = 0;
	colors_line = *rgb;
	if (!colors_line || !ft_isdigit((unsigned char)*colors_line))
		return (-1);
	while (*colors_line && ft_isdigit((unsigned char)*colors_line))
	{
		value = value * 10 + (*colors_line - '0');
		if (value > 255)
			return (-1);
		colors_line++;
	}
	*rgb_number = (int)value;
	*rgb = colors_line;
	return (0);
}

int	parse_color_value(const char *rgb_numbers, int rgb_number[3])
{
	const char	*rgb;

	rgb = rgb_numbers + 1;
	while (*rgb == ' ' || *rgb == '\t')
		rgb++;
	if (parse_uint0_255(&rgb, &rgb_number[0]) < 0)
		return (-1);
	if (*rgb != ',')
		return (-1);
	rgb++;
	if (parse_uint0_255(&rgb, &rgb_number[1]) < 0)
		return (-1);
	if (*rgb != ',')
		return (-1);
	rgb++;
	if (parse_uint0_255(&rgb, &rgb_number[2]) < 0)
		return (-1);
	while (*rgb == ' ' || *rgb == '\t')
		rgb++;
	if (*rgb != '\0' && *rgb != '\n')
		return (-1);
	return (0);
}

int	process_header(int id, t_game *g, char *header_line)
{
	if ((id == 0 && (g->has_no || parse_texture_path(&g->no, header_line) < 0))
		|| (id == 1 && (g->has_so || parse_texture_path(&g->so,
					header_line) < 0))
		|| (id == 2 && (g->has_we || parse_texture_path(&g->we,
					header_line) < 0))
		|| (id == 3 && (g->has_ea || parse_texture_path(&g->ea,
					header_line) < 0))
		|| (id == 4 && (g->has_f || parse_color_value(header_line,
					g->floor) < 0))
		|| (id == 5 && (g->has_c
				|| parse_color_value(header_line, g->ceiling) < 0)))
		return (-1);
	if (id == 0)
		g->has_no = 1;
	else if (id == 1)
		g->has_so = 1;
	else if (id == 2)
		g->has_we = 1;
	else if (id == 3)
		g->has_ea = 1;
	else if (id == 4)
		g->has_f = 1;
	else if (id == 5)
		g->has_c = 1;
	return (0);
}
