/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:20:21 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/11/17 17:48:26 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"

typedef struct s_texture
{
	void	*imagen;
	char	*addr;
	int		width;
	int		height;
	int		bit_by_pixel;
	int		line_len_byte;
	int		endian;
}			t_texture;

typedef struct s_game
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		has_no;
	int		has_so;
	int		has_we;
	int		has_ea;
	int		floor[3];
	int		ceiling[3];
	int		has_floor;
	int		has_ceiling;
	int		map_height;
	int		map_width;
	char	**map;
	void		*mlx;
	void		*window;
	t_texture		texture_no;
	t_texture		texture_so;
	t_texture		texture_we;
	t_texture		texture_ea;

		/* pantalla */
	int screen_w;
	int screen_h;

	/* jugador / cámara */
	double posx;
	double posy;
	double dirx;
	double diry;
	double planex;
	double planey;

	/* input */
	int key_w;
	int key_s;
	int key_a;
	int key_d;
	int key_left;
	int key_right;

	/* frame buffer (imagen) */
	void *frame_img;
	char *frame_addr;
	int frame_bpp;
	int frame_line_len;
	int frame_endian;

}			t_game;

#endif