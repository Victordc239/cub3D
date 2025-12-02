/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:47:00 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/02 17:31:19 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_texture(char **dst, const char *rest)
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
	*dst = copy;
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

int	apply_header_action(int idx, t_game *g, char *trim)
{
	if ((idx == 0 && (g->has_no || set_texture(&g->no, trim + 2) < 0))
		|| (idx == 1 && (g->has_so || set_texture(&g->so, trim + 2) < 0))
		|| (idx == 2 && (g->has_we || set_texture(&g->we, trim + 2) < 0))
		|| (idx == 3 && (g->has_ea || set_texture(&g->ea, trim + 2) < 0))
		|| (idx == 4 && (g->has_f || parse_color_value(trim + 1, g->floor) < 0))
		|| (idx == 5 && (g->has_c
				|| parse_color_value(trim + 1, g->ceiling) < 0)))
		return (-1);
	if (idx == 0)
		g->has_no = 1;
	else if (idx == 1)
		g->has_so = 1;
	else if (idx == 2)
		g->has_we = 1;
	else if (idx == 3)
		g->has_ea = 1;
	else if (idx == 4)
		g->has_f = 1;
	else if (idx == 5)
		g->has_c = 1;
	return (0);
}
