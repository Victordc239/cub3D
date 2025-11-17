# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <victor@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/03 15:23:00 by sofernan          #+#    #+#              #
#    Updated: 2025/11/16 15:49:51 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = libft_obj

SRCS = \
ft_atoi.c \
ft_bzero.c \
ft_calloc.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isdigit.c \
ft_itoa.c \
ft_memcpy.c \
ft_memset.c \
ft_putchar.c \
ft_putstr.c \
ft_split.c \
ft_strchr.c \
ft_strdup.c \
ft_strjoin.c \
ft_strjoin_gnl.c \
ft_strlen.c \
ft_strncmp.c \
ft_strcmp.c \
ft_substr.c \
get_next_line.c \
ft_strcpy.c \
ft_strcat.c \
ft_strtrim.c

OBJS = $(SRCS:.c=.o)
OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(OBJS))

NAME = libft.a

TOTAL_FILES := $(words $(SRCS))
COUNT = 0

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_FILES)
	@ar rcs $(NAME) $(OBJ_FILES)
	@echo "\033[1;33mLeaving Libft...\033[0m"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(eval COUNT=$(shell echo $$(( $(COUNT) + 1 ))))
	$(eval PERCENT=$(shell echo $$(( $(COUNT) * 100 / $(TOTAL_FILES) ))))
	@echo "\033[1;32m[ $(PERCENT)%] Building C object $@\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ_FILES)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re