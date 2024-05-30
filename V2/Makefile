# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/23 11:31:47 by jqueijo-          #+#    #+#              #
#    Updated: 2024/05/30 15:36:48 by jqueijo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = main.c \
	parsing.c \
	str_utils.c \
	init_data.c \
	cleanup.c \
	init_dinner.c \
	utils.c \
	routine.c \
	monitor.c \

CC = cc -pthread
CFLAGS = -Wall -Werror -Wextra
SANITIZE = -g -fsanitize=thread

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

debug: $(CFLAGS) += $(SANITIZE)
debug: re

re: fclean all

.PHONY: all, clean, fclean, re, debug
