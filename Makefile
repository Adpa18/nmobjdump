CC	=	gcc

RM	=	rm -rf

CFLAGS		= -W -Wall -Wextra -Werror -I./inc

SRC			=	src/file.c	\
				src/sym32.c	\
				src/sym64.c	\
				src/elf.c

SRC_NM		=	nm/main.c

SRC_OBJDUMP	=

SRC_NM		+= $(SRC)

SRC_OBJDUMP	+= $(SRC)

NAME_NM			=	my_nm

NAME_OBJDUMP	=	my_objdump

OBJ_NM		=	$(SRC_NM:.c=.o)

OBJ_OBJDUMP	=	$(SRC_OBJDUMP:.c=.o)

all	: $(NAME_NM) $(NAME_OBJDUMP)

$(NAME_NM)		:	$(OBJ_NM)
	$(CC) $(OBJ_NM) -o $(NAME_NM)

$(NAME_OBJDUMP)	:	$(OBJ_OBJDUMP)
	$(CC) $(OBJ_OBJDUMP) -o $(NAME_OBJDUMP)

nm		:	$(NAME_NM)

objdump	:	$(NAME_OBJDUMP)

clean	:
	$(RM) $(OBJ_NM) $(OBJ_OBJDUMP)

fclean	:	clean
	$(RM) $(NAME_NM) $(NAME_OBJDUMP)

re		:	fclean all

.PHONY	:	all nm objdump clean fclean re
