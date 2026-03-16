##
## EPITECH PROJECT, 2026
## arcade
## File description:
## Makefile
##

SRC =		$(addprefix src/, \
 				main.cpp \
				$(addprefix Core/, \
					Core.cpp \
				)\
			)

OBJ =		$(SRC:.cpp=.o)

NAME =		arcarde

all: 		$(NAME)

CXX 		?= clang++

CXXFLAGS 	+= -Wall -Wextra

CPPFLAGS 	+= -I include/

LIBFLAG 	+= -ldl

$(NAME): 	$(OBJ)
			$(CXX) $(OBJ) -o $(NAME) $(LIBFLAG)

clean:
			$(RM) $(OBJ)

fclean: 	clean
			$(RM) $(NAME)

re: 		fclean all

.PHONY: 	all clean fclean re
