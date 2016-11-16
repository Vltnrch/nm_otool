# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vroche <vroche@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/03/16 10:49:38 by vroche            #+#    #+#              #
#    Updated: 2016/11/16 17:57:12 by vroche           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = nm_otool

NAME_NM = ft_nm

NAME_OTOOL = ft_otool

LIB = -L./libft -lft

HDR = -I./libft/includes -I./includes

FLAGS = -Wall -Wextra -Werror -g

C_NM =	srcs/nm/ft_nm.c \
		srcs/nm/nm_32.c \
		srcs/nm/nm_64.c \
		srcs/nm/nm_ar.c \
		srcs/nm/nm_fat.c \
		srcs/nm/nm_print.c

O_NM = $(C_NM:.c=.o)

C_OTOOL =	srcs/otool/ft_otool.c \
			srcs/otool/otool_32.c \
			srcs/otool/otool_64.c \
			srcs/otool/otool_ar.c \
			srcs/otool/otool_fat.c

O_OTOOL = $(C_OTOOL:.c=.o)

C_COMMON =	srcs/common/nm_otool_list_32.c \
			srcs/common/nm_otool_list_64.c \
			srcs/common/nm_otool_list_ar.c

O_COMMON = $(C_COMMON:.c=.o)

C = $(C_NM) $(C_OTOOL) $(C_COMMON)

O = $(O_NM) $(O_OTOOL) $(O_COMMON)

all: $(NAME)

$(NAME): $(NAME_NM) $(NAME_OTOOL)

$(NAME_NM): $(O_COMMON) $(O_NM) 
	@make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $^ -o $@

$(NAME_OTOOL): $(O_COMMON) $(O_OTOOL)
	@make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $^ -o $@

%.o:%.c
	clang $(FLAGS) $(HDR) -c $< -o $@

clean:
	make -C ./libft clean
	rm -f $(O)

fclean: clean
	make -C ./libft fclean
	rm -f $(NAME_NM) $(NAME_OTOOL)

re: fclean all

.PHONY: all clean fclean re libft
