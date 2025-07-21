CC = gcc
SRC = main.c \
parsing.c\
utils.c\

SRC_BONUS = 

OBJ = ${SRC:.c=.o}

OBJ_BONUS = ${SRC_BONUS:.c=.o}

LFLAGS = -Llibft -lft

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LIBFT = libft/libft.a
HDR = malcolm.h
HDRB = malcolm_bonus.h
NAME = malcolm

ifndef WithBonus
	OBJS = $(OBJ)
else
	OBJS = $(OBJ_BONUS)
endif

all: $(NAME)

$(LIBFT):
	@make -C ./libft bonus

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)
ifndef WithBonus
	rm -f $(OBJ_BONUS)
else
	rm -f $(OBJ)
endif

.c.o: $(HDR) Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

bonus:
	@make WithBonus=1

clean:
	rm -f $(OBJ)
	rm -f $(OBJ_BONUS)
	cd libft && make clean;

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)
	rm -f $(LIBFT)

re: fclean all

.PHONY: all clean flcean re bonus