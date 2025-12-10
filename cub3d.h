/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:20:21 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/12/10 18:07:31 by sofernan         ###   ########.fr       */
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
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	int			has_no;
	int			has_so;
	int			has_we;
	int			has_ea;
	int			floor[3];
	int			ceiling[3];
	int			has_f;
	int			has_c;
	int			map_height;
	int			map_width;
	int			y;
	char		**map;
	void		*mlx;
	void		*window;
	double		now;

	/* textures */
	t_texture	texture_no;
	t_texture	texture_so;
	t_texture	texture_we;
	t_texture	texture_ea;

	/* screen */
	int			screen_w;
	int			screen_h;

	/* player */
	double		posx;
	double		posy;
	double		dirx;
	double		diry;
	double		planex;
	double		planey;
	int			init_player_x;
	int			init_player_y;

	/* input */
	int			key_w;
	int			key_s;
	int			key_a;
	int			key_d;
	int			key_left;
	int			key_right;

	/* frame buffer */
	void		*frame_img;
	char		*frame_addr;
	int			frame_bpp;
	int			frame_line_len;
	int			frame_endian;

	/* mouse */
	int			mouse_last_x;
	int			mouse_last_y;

	/* parse */
	size_t		npos;
	size_t		x_neighbor;
	size_t		y_neighbor;

}			t_game;

typedef struct s_render
{
	unsigned int	color;
	char			mch;
	int				x;
	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				tex_width;
	int				tex_height;
	int				tex_x;
	int				tex_y;
	double			camera_x;
	double			ray_dir_x;
	double			ray_dir_y;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	double			wall_dist;
	double			wall_x;
	double			step;
	double			tex_pos;
	double			denom;
}			t_render;

void		free_config(t_game *g);
void		free_copy(char **copy);
void		free_map(t_game *g);
void		strip_nl(char *line);
void		draw_frame_background(t_game *g);
void		init_player(t_game *g, char orient);
void		update_player(t_game *g, double delta);
void		draw_pixel_frame(t_game *g, int x, int y, unsigned int color);
void		render_frame(t_game *g, t_render *r, t_texture **tex);
void		init_struct_g(t_game *g);
void		move_left_right(t_game *g, double move_speed, int direction);
void		move_forward_back(t_game *g, double move_speed, int direction);
void		rotate_player(t_game *g, double angle);
void		free_image(void *mlx, void **img_ptr, char **addr_ptr);
void		drain_gnl_fd(int fd);
void		init_render_vars(t_game *g, t_render *r, int x);
void		init_ray_steps(t_game *g, t_render *r);
void		cast_ray(t_game *g, t_render *r);
void		calculate_wall_projection(t_game *g, t_render *r);
void		render_column(t_game *g, t_render *r, t_texture *tex);

int			extension_is_cub(const char *fname);
int			check_char(char c, int flag_walkable);
int			flood_fill(t_game *g, char **visited, int y, int x);
int			check_map_closed(t_game *g);
int			validate_map_chars(t_game *g);
int			parse_map(int fd, char *first_line, t_game *g);
int			is_empty_line(const char *line);
int			parse_uint0_255(const char **rgb, int *rgb_number);
int			parse_color_value(const char *rgb_numbers, int rgb_number[3]);
int			parse_texture_path(char **dest, const char *textures_path);
int			is_header_line(const char *line);
int			parse_headers(int fd, t_game *g, char **first_map_line);
int			mouse_move(int x, int y, void *param);
int			pad_map(t_game *g, int y);
int			find_player(t_game *g, char *out_orient);
int			key_press(int keycode, void *param);
int			handle_close(void *param);
int			load_texture(void *mlx, t_texture *texture, char *path);
int			get_texture_pixel(t_texture *texture, int x, int y);
int			create_frame_image(t_game *g);
int			key_release(int keycode, void *param);
int			game_loop(void *param);
int			parse(char **argv, t_game *g, char *orient);
int			check_file_empty(const char *path);
int			check_empty_lines_in_map(t_game *g);
int			parse_header_order(const char *line, t_game *g, int *order);
int			build_map_from_lines(t_game *g, char **lines, size_t count);
int			grow_lines(char ***lines, size_t *cap, size_t count, char *line);
int			check_all_visited(t_game *g, char *visited, size_t w, size_t h);
int			bfs_from_start(t_game *g, char *visited, int *queue, size_t start);
int			find_player_pos(t_game *g, int *out_py, int *out_px);
int			process_header(int id, t_game *g, char *header_line);
int			process_neighbor(t_game *g, char *visited, int *queue,
				size_t *tail);
int			find_player_in_copy(char **copy, int map_height, int *out_y,
				int *out_x);
int			process_neighbors_of(t_game *g, char *visited, int *queue,
				size_t *tail);
int			process_headers(int fd, t_game *g,
				char **first_map_line, int order[6]);
int			read_first_map_line(int fd, t_game *g,
				char **first_map_line, int order[6]);
int			allocate_visited_and_queue(long long total, char **visited_out,
				int **queue_out);

char		*read_rest_of_file(int fd);

double		get_current_time_s(void);

t_texture	*choose_wall_texture(t_game *g, t_render *r);

#endif