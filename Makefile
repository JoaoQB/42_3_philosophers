# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/17 12:19:57 by jqueijo-          #+#    #+#              #
#    Updated: 2024/05/22 13:54:30 by jqueijo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = main.c \
	parsing.c \
	string_utils.c \
	dinner_init.c \
	dinner_run.c \
	utils.c \
	routine.c \
	monitor.c

CC = cc -pthread
CFLAGS = -Wall -Werror -Wextra
SANITIZER = -g -fsanitize=thread

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

sanitize: CFLAGS += $(SANITIZER)
sanitize: re

re: fclean all

.PHONY: all, clean, fclean, re, dummy
