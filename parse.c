/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:58:28 by sofernan          #+#    #+#             */
/*   Updated: 2025/12/15 13:51:30 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_file_extension(const char *fname)
{
	int	len;

	if (!fname)
		return (0);
	len = ft_strlen(fname);
	if (len < 4)
		return (0);
	return (ft_strcmp(fname + len - 4, ".cub") == 0);
}

int	check_file_empty(const char *path)
{
	int		fd;
	int		result;
	char	tmp[1];

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror("Error\nOpen"), -1);
	result = read(fd, tmp, 1);
	if (result < 0)
	{
		close(fd);
		return (perror("Error\nRead"), -1);
	}
	if (result == 0)
	{
		close(fd);
		return (write(1, "Error\nEmpty file\n", 17), -1);
	}
	close(fd);
	return (0);
}

void	init_struct_g(t_game *g)
{
	if (!g)
		return ;
	ft_memset(g, 0, sizeof(*g));
	g->screen_w = 1920;
	g->screen_h = 1080;
	g->mlx = mlx_init();
}

void	clear_gnl_fd(int fd)
{
	char	*tmp;

	if (fd < 0)
		return ;
	tmp = get_next_line(fd);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
}

int	parse(char **argv, t_game *g, char *orient)
{
	int		fd;
	char	*first_map_line;

	first_map_line = NULL;
	if (!check_file_extension(argv[1]))
		return (write(2, "Error\nExtension\n", 16), -1);
	if (check_file_empty(argv[1]) < 0)
		return (-1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Error\nOpen"), -1);
	init_struct_g(g);
	if (!g->mlx)
		return (close(fd), perror("Error\nmlx_init failed\n"),
			free_map(g), free_config(g), -1);
	if (parse_headers(fd, g, &first_map_line) < 0)
		return (clear_gnl_fd(fd), close(fd), free_config(g),
			write(2, "Error\nHeader\n", 13), -1);
	if (parse_map(fd, first_map_line, g) < 0)
		return (close(fd), free_config(g), -1);
	if (check_empty_lines_map(g) < 0 || complete_map_rows(g, 0) < 0
		|| validate_map_chars(g) < 0 || validate_map_closed(g) < 0
		|| init_player_pos(g, orient) < 0)
		return (free_map(g), free_config(g), close(fd), -1);
	return (close(fd), 0);
}
