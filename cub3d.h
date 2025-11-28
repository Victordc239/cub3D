/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <victor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:20:21 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/11/28 12:47:24 by victor           ###   ########.fr       */
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
# include <math.h>
# include <sys/time.h>
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
	int	screen_w;
	int	screen_h;

	/* jugador / cámara */
	double	posx;
	double	posy;
	double	dirx;
	double	diry;
	double	planex;
	double	planey;

	/* input */
	int	key_w;
	int	key_s;
	int	key_a;
	int	key_d;
	int	key_left;
	int	key_right;

	/* frame buffer (imagen) */
	void	*frame_img;
	char	*frame_addr;
	int	frame_bpp;
	int	frame_line_len;
	int	frame_endian;

	/*mouse*/
	int	mouse_last_x;
	int	mouse_last_y;

}			t_game;

void	free_config(t_game *g);
void	free_copy(char **copy);
void	free_map(t_game *g);
void	strip_nl(char *s);
void	draw_background_to_frame(t_game *g);
void	init_player(t_game *g, int py, int px, char orient);
void	update_player(t_game *g, double delta);
void	put_pixel_frame(t_game *g, int x, int y, unsigned int color);
void	render_frame(t_game *g);
void	init_struct_g(t_game *g);

int		extension_is_cub(const char *fname);
int		check_char(char c, int flag_walkable);
int		flood_fill(t_game *g, char **visited, int y, int x);
int		check_map_closed(t_game *g);
int		validate_map_chars(t_game *g);
int		parse_map(int fd, char *first_line, t_game *g);
int		is_blank_line(const char *s);
int		parse_uint0_255(const char **p, int *out);
int		parse_color_values(const char *rest, int out_rgb[3]);
int		set_texture(char **dst, const char *rest);
int		is_header_line(const char *line);
int		parse_headers(int fd, t_game *g, char **out_first_map_line);
int		mouse_move(int x, int y, void *param);
int		pad_map(t_game *g);
int		find_player(t_game *g, int *out_y, int *out_x, char *out_orient);
int		key_press(int keycode, void *param);
int		handle_close(void *param);
int		load_texture(void *mlx, t_texture *texture, char *path);
int		get_tex_pixel(t_texture *texture, int x, int y);
int		create_frame(t_game *g);
int		key_release(int keycode, void *param);
int		game_loop(void *param);
int		parse(char **argv, t_game *g, int *py, int *px, char *orient);

char	**copy_map(t_game *g);
char	*read_rest_of_file(int fd);

double	get_time_s(void);

t_texture	*choose_wall_texture(t_game *g, int side, double ray_dir_x, double ray_dir_y);

#endif