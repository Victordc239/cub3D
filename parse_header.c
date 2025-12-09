/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:47:00 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/09 17:33:55 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_texture_path(char **dest, const char *rest)
{
	const char	*p;
	char		*copy;

	p = rest;
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p == '\0' || *p == '\n')
		return (-1);
	copy = ft_strdup(p);
	if (!copy)
		return (-1);
	strip_nl(copy);
	*dest = copy;
	return (0);
}

int	parse_uint0_255(const char **p, int *out)
{
	long		v;
	const char	*s;

	v = 0;
	s = *p;
	if (!s || !ft_isdigit((unsigned char)*s))
		return (-1);
	while (*s && ft_isdigit((unsigned char)*s))
	{
		v = v * 10 + (*s - '0');
		if (v > 255)
			return (-1);
		s++;
	}
	*out = (int)v;
	*p = s;
	return (0);
}

int	parse_color_value(const char *rest, int out_rgb[3])
{
	const char	*p;

	p = rest;
	while (*p == ' ' || *p == '\t')
		p++;
	if (parse_uint0_255(&p, &out_rgb[0]) < 0)
		return (-1);
	if (*p != ',')
		return (-1);
	p++;
	if (parse_uint0_255(&p, &out_rgb[1]) < 0)
		return (-1);
	if (*p != ',')
		return (-1);
	p++;
	if (parse_uint0_255(&p, &out_rgb[2]) < 0)
		return (-1);
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p != '\0' && *p != '\n')
		return (-1);
	return (0);
}

int	process_header(int id, t_game *g, char *trim)
{
	if ((id == 0 && (g->has_no || parse_texture_path(&g->no, trim + 2) < 0))
		|| (id == 1 && (g->has_so || parse_texture_path(&g->so, trim + 2) < 0))
		|| (id == 2 && (g->has_we || parse_texture_path(&g->we, trim + 2) < 0))
		|| (id == 3 && (g->has_ea || parse_texture_path(&g->ea, trim + 2) < 0))
		|| (id == 4 && (g->has_f || parse_color_value(trim + 1, g->floor) < 0))
		|| (id == 5 && (g->has_c
				|| parse_color_value(trim + 1, g->ceiling) < 0)))
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
