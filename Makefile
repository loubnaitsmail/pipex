 
NAME	=	pipex


CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -g
IFLAGS	=	-I. -I./libft -I includes
LFLAGS	=	-L./libft -lft
 
SRC	=				srcs/mult_pipe.c \

MUTUEL =	srcs/execute_cmd.c \

SRC_BONUS	= 	srcs/mult_pipe_bonus.c \

OBJ		=	$(SRC:%.c=%.o)

OBJ_MUT		=	$(MUTUEL:%.c=%.o)

OBJ_BONUS		=	$(SRC_BONUS:%.c=%.o)
 
%.o: %.c
			$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
 
ifeq ($(shell uname), Linux)
$(NAME): IFLAGS = -I. -I./libft
endif
 
$(NAME):	$(OBJ) $(OBJ_MUT)
			$(MAKE) -C libft
			$(CC) $(CFLAGS) $(OBJ) $(OBJ_MUT) $(IFLAGS) $(LFLAGS) -o $@
 
 
all:		$(NAME)
 
bonus:		$(OBJ_BONUS) $(OBJ_MUT)
			$(MAKE) -C libft
			$(CC) $(CFLAGS) $(OBJ_BONUS) $(OBJ_MUT) $(IFLAGS) $(LFLAGS) -o $(NAME)
 
clean:
			$(MAKE) -C libft clean
			rm -rf $(OBJ) $(OBJ_MUT) $(OBJ_BONUS)
 
ifeq ($(shell uname), Linux)
endif
 
fclean:		clean
			$(MAKE) -C libft fclean
			rm -rf $(NAME)
 
re:			fclean all
 
.PHONY: all clean fclean re
 
