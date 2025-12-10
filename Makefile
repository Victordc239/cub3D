# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 13:49:21 by vdiez-cu          #+#    #+#              #
#    Updated: 2025/12/10 16:17:39 by vdiez-cu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

SRCS		= main.c parse_header.c parse_header_order.c 3.c 4.c 5.c 6.c 7.c 8.c 9.c 10.c \
			  11.c 12.c 13.c

SRCS_BONUS	= main_bonus.c parse_header.c parse_header_order.c 3.c 4_bonus.c 5.c 6.c 7.c 8.c 9.c 10.c \
			  11.c 12.c 13_bonus.c

LIBFT = libft
LIBFT_DIR = libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a

OBJS		= $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

MLX_DIR		= ./minilibx-linux
MLX_LIB		= $(MLX_DIR)/libmlx.a

MLX_FLAGS	= -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

CC		= cc
CFLAGS	= -Wall -Werror -Wextra

all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME) $(LIBFT_LIB)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR) all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJS_BONUS)
	@$(MAKE) -C $(MLX_DIR) clean
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(LIBFT_LIB)

bonus: fclean $(MLX_LIB) $(LIBFT_LIB) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(MLX_FLAGS) -o $(NAME) $(LIBFT_LIB)

re: fclean all

.PHONY: all clean fclean re bonus