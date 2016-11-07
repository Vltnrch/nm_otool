# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vroche <vroche@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/03/16 10:49:38 by vroche            #+#    #+#              #
#    Updated: 2016/11/07 14:14:01 by vroche           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = nm_otool

NAME_NM = ft_nm

NAME_OTOOL = ft_otool

LIB = -L./libft -lft

HDR = -I./libft/includes -I.

FLAGS = -Wall -Wextra -Werror -g

C_NM =	ft_list.c \
		ft_nm.c \
		nm_32.c \
		nm_64.c \
		nm_fat.c \
		nm_print.c

O_NM = $(C_NM:.c=.o)

C_OTOOL = otool.c

O_OTOOL = $(C_OTOOL:.c=.o)

C = $(C_NM) $(C_OTOOL)

O = $(O_NM) $(O_OTOOL)

all: $(NAME)

$(NAME): $(NAME_NM) $(NAME_OTOOL)

$(NAME_NM): $(O_NM)
	make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $(O_NM) -o $(NAME_NM)

$(NAME_OTOOL): $(O_OTOOL)
	make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $(O_OTOOL) -o $(NAME_OTOOL)

%.o:%.c
	clang $(FLAGS) $(HDR) -c $<

clean:
	make -C ./libft clean
	rm -f $(O)

fclean: clean
	make -C ./libft fclean
	rm -f $(NAME_NM) $(NAME_OTOOL)

re: fclean all

.PHONY: all clean fclean re libft
