##
## EPITECH PROJECT, 2026
## arcade
## File description:
## Makefile for arcade project
##

CXX				?= clang++
CXXFLAGS		+= -Wall -Wextra -fPIC -fno-gnu-unique
CPPFLAGS		+= -I include/
LDFLAGS			+= -ldl

CORE_SRC		=	$(addprefix src/
						main.cpp \
						$(addprefix Core/
							Core.cpp \
						)\
						$(addprefix ErrorHandling/
							errorHandling.cpp \
						)\
					)

CORE_OBJ		=	$(CORE_SRC:.cpp=.o)

ARCADE			=	arcade

LIB_DIR			=	lib

GAMES_SRC		=	$(addprefix src/,
						Snake.cpp \
					)

GAMES_SO		=	$(LIB_DIR)/arcade_snake.so

GRAPHICS_SRC	=	$(addprefix src/
						NcursesGraph.cpp \
						SfmlGraph.cpp \
					)

GRAPHICS_SO		=	$(LIB_DIR)/arcade_ncurses.so \
					$(LIB_DIR)/arcade_sfml.so

all:			$(ARCADE) games graphicals

lib:
				mkdir -p $(LIB_DIR)

$(ARCADE):		$(CORE_OBJ)
				$(CXX) $(CORE_OBJ) -o $(ARCADE) $(LDFLAGS)

%.o:			%.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB_DIR)/arcade_snake.so: lib src/Snake.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -shared src/Snake.cpp -o $@

$(LIB_DIR)/arcade_ncurses.so: lib src/NcursesGraph.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -shared src/NcursesGraph.cpp -o $@ -lncurses

$(LIB_DIR)/arcade_sfml.so: lib src/SfmlGraph.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -shared src/SfmlGraph.cpp -o $@ $(shell pkg-config --cflags --libs sfml-graphics 2>/dev/null || echo "-lsfml-graphics -lsfml-window -lsfml-system")

core:			$(ARCADE)
				@echo "✓ Core arcade executable built"

games:			lib $(GAMES_SO)
				@echo "✓ Games libraries built"

graphicals:		lib $(GRAPHICS_SO)
				@echo "✓ Graphical libraries built"

clean:
				$(RM) $(CORE_OBJ)

fclean:			clean
				$(RM) $(ARCADE)
				$(RM) -r $(LIB_DIR)

re:				fclean all

.PHONY:			all core games graphicals lib clean fclean re
