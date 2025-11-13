# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 13:49:21 by vdiez-cu          #+#    #+#              #
#    Updated: 2025/11/13 14:48:43 by vdiez-cu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= so_long

SRCS		= main.c
			

LIBFT = libft
LIBFT_DIR = libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a

OBJS		= $(SRCS:.c=.o)

MLX_DIR		= ./minilibx-linux
MLX_LIB		= $(MLX_DIR)/libmlx.a

MLX_FLAGS	= -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

CC		= cc
CFLAGS	= -Wall -Werror -Wextra

all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

$(LIBFT_LIB):
	@echo "\033[1;33mCompiling libft...\033[0m"
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME) $(LIBFT_LIB)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR) all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(MLX_DIR) $(LIBFT_DIR) clean

fclean:
	rm -f $(OBJS)
	$(MAKE) -C $(MLX_DIR) $(LIBFT_DIR) clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re