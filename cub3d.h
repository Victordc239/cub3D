/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:20:21 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/12/15 15:28:32 by sofernan         ###   ########.fr       */
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
	int			row_y;
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

}				t_game;

typedef struct s_render
{
	unsigned int	color;
	char			mch;
	int				col_x;
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
}					t_render;

int			parse_texture_path(char **dest, const char *textures_path);
int			parse_uint0_255(const char **rgb, int *rgb_number);
int			parse_color_value(const char *rgb_numbers, int rgb_number[3]);
int			process_header(int id, t_game *g, char *header_line);
int			get_header_id(const char *header_line);
int			check_header_order(int *order, int id);
int			parse_header_order(const char *line, t_game *g, int *order);
int			is_empty_line(const char *line);
int			is_header_line(const char *line);
int			parse_headers(int fd, t_game *g, char **first_map_line);
int			get_texture_pixel(t_texture *texture, int col_x, int row_y);
int			create_frame_image(t_game *g);
int			game_loop(void *param);
int			build_map(t_game *g, char **lines, size_t line_count);
int			parse_map(int fd, char *first_line, t_game *g);
int			find_player_pos(t_game *g, int *player_y, int *player_x);
int			validate_map_closed(t_game *g);
int			check_empty_lines_map(t_game *g);
int			complete_map_rows(t_game *g, int row_y);
int			is_valid_char(char c, int walkable_char);
int			validate_map_chars(t_game *g);
int			init_player_pos(t_game *g, char *player_orientation);
int			check_file_extension(const char *fname);
int			check_file_empty(const char *path);
int			parse(char **argv, t_game *g, char *orient);
int			close_game(void *param);
int			key_press(int keycode, void *param);
int			key_release(int keycode, void *param);
int			rotate_player_mouse(int col_x, int row_y, void *param);
int			load_texture(void *mlx, t_texture *texture, char *path);
int			read_first_map_line(int fd, t_game *g,
				char **first_map_line, int order[6]);
int			process_headers(int fd, t_game *g,
				char **first_map_line, int order[6]);
int			resize_lines(char ***lines, size_t *lines_capacity,
				size_t line_count, char *map_line);
int			process_neighbor(t_game *g, char *visited_cells, int *pending_cells,
				size_t *tail);
int			process_neighbors(t_game *g, char *visited_cells,
				int *pending_cells, size_t *tail);
int			validate_map(t_game *g, char *visited_cells, int *pending_cells,
				size_t start_cell);
int			alloc_bfs_data(long long total_cells, char **visited_cells_out,
				int **pending_cells_out);
int			check_all_cells_visited(t_game *g, char *visited_cells,
				size_t width, size_t height);
int			init_player_pos_in_copy(char **copy, int map_height, int *out_y,
				int *out_x);

void		move_forward_back(t_game *g, double move_speed, int direction);
void		move_left_right(t_game *g, double move_speed, int direction);
void		rotate_player(t_game *g, double angle);
void		update_player(t_game *g, double delta);
void		render_column(t_game *g, t_render *r, t_texture *tex);
void		init_render_vars(t_game *g, t_render *r, int col_x);
void		init_ray_steps(t_game *g, t_render *r);
void		cast_ray(t_game *g, t_render *r);
void		calculate_wall_projection(t_game *g, t_render *r);
void		draw_frame_background(t_game *g);
void		render_frame(t_game *g, t_render *r, t_texture **tex);
void		strip_newline(char *map_line);
void		init_struct_g(t_game *g);
void		clear_gnl_fd(int fd);
void		free_map(t_game *g);
void		free_image(void *mlx, void **img_ptr, char **addr_ptr);
void		free_config(t_game *g);
void		init_player(t_game *g, char orient);
void		draw_pixel_frame(t_game *g, int col_x, int row_y,
				unsigned int color);

char		*read_rest_of_file(int fd);

double		get_current_time_s(void);

t_texture	*choose_wall_texture(t_game *g, t_render *r);

#endif