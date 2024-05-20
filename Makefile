# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/17 12:19:57 by jqueijo-          #+#    #+#              #
#    Updated: 2024/05/20 11:22:33 by jqueijo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = main.c \
	parsing.c \
	string_utils.c \
	init.c \
	dining.c \
	utils.c \
	routine.c \
	# cleanup.c \

CC = cc
CFLAGS =

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

real: CFLAGS += -Wall -Werror -Wextra -O3 #DON'T FORGET TO CHANGE
real: re

re: fclean all

.PHONY: all, clean, fclean, re, dummy
