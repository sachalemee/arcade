##
## EPITECH PROJECT, 2026
## arcade
## File description:
## Makefile for arcade project
##

CXX				?= clang++
CXXFLAGS		+= -Wall -Wextra -g3 -fPIC -fno-gnu-unique
CPPFLAGS		+= -I include/
LDFLAGS			+= -ldl

CORE_SRC		=	src/main.cpp \
					src/Core/Core.cpp \
					src/ErrorHandling/errorHandling.cpp \
					src/LibManager/LibManager.cpp

CORE_OBJ		=	$(CORE_SRC:.cpp=.o)

ARCADE			=	arcade

LIB_DIR			=	lib

GRAPHICS_NCURSES_SRC	=	src/NCurses/NCursesDisplay.cpp
GRAPHICS_SDL2_SRC		=	src/SDL2/SDL2Display.cpp
GRAPHICS_SFML_SRC		=	src/SFML/SFMLDisplay.cpp

GRAPHICS_NCURSES_OBJ	=	$(GRAPHICS_NCURSES_SRC:.cpp=.o)
GRAPHICS_SDL2_OBJ		=	$(GRAPHICS_SDL2_SRC:.cpp=.o)
GRAPHICS_SFML_OBJ		=	$(GRAPHICS_SFML_SRC:.cpp=.o)

GRAPHICS_SO				=	$(LIB_DIR)/arcade_ncurses.so \
							$(LIB_DIR)/arcade_sdl2.so \
							$(LIB_DIR)/arcade_sfml.so

GAMES_MINESWEEPER_SRC	= 	games/Minesweeper.cpp
GAMES_SNAKE_SRC			= 	games/Snake.cpp

GAMES_MINESWEEPER_OBJ	= 	$(GAMES_MINESWEEPER_SRC:.cpp=.o)
GAMES_SNAKE_OBJ			= 	$(GAMES_SNAKE_SRC:.cpp=.o)

GAMES_SO		=	$(LIB_DIR)/arcade_minesweeper.so \
					$(LIB_DIR)/arcade_snake.so

MENU_SRC 		= 	src/menu/menu.cpp
MENU_OBJ 		= 	$(MENU_SRC:.cpp=.o)

MENU_SO			=	$(LIB_DIR)/arcade_menu.so

all:			$(ARCADE) graphicals games

lib:
				mkdir -p $(LIB_DIR)

$(ARCADE):		$(CORE_OBJ) lib
				$(CXX) $(CORE_OBJ) -o $(ARCADE) $(LDFLAGS)
				@echo "✓ Core arcade executable built: $(ARCADE)"

%.o:			%.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB_DIR)/arcade_ncurses.so: $(GRAPHICS_NCURSES_OBJ) lib
				$(CXX) $(CXXFLAGS) -shared $(GRAPHICS_NCURSES_OBJ) -o $@ -lncurses
				@echo "✓ Built: $@"

$(LIB_DIR)/arcade_sdl2.so: $(GRAPHICS_SDL2_OBJ) lib
				$(CXX) $(CXXFLAGS) -shared $(GRAPHICS_SDL2_OBJ) -o $@ $(shell pkg-config --cflags --libs sdl2 SDL2_ttf 2>/dev/null || echo "-lSDL2 -lSDL2_ttf")
				@echo "✓ Built: $@"

$(LIB_DIR)/arcade_sfml.so: $(GRAPHICS_SFML_OBJ) lib
				$(CXX) $(CXXFLAGS) -shared $(GRAPHICS_SFML_OBJ) -o $@ $(shell pkg-config --cflags --libs sfml-graphics 2>/dev/null || echo "-lsfml-graphics -lsfml-window -lsfml-system")
				@echo "✓ Built: $@"

$(LIB_DIR)/arcade_minesweeper.so: $(GAMES_MINESWEEPER_OBJ) lib
				$(CXX) $(CXXFLAGS) -shared $(GAMES_MINESWEEPER_OBJ) -o $@
				@echo "✓ Built: $@"

$(LIB_DIR)/arcade_snake.so: $(GAMES_SNAKE_OBJ) lib
				$(CXX) $(CXXFLAGS) -shared $(GAMES_SNAKE_OBJ) -o $@
				@echo "✓ Built: $@"

$(MENU_SO): $(MENU_OBJ) lib
				$(CXX) $(CXXFLAGS) -shared $(MENU_OBJ) -o $@
				@echo "✓ Built: $@"

core:			$(ARCADE)
				@echo "✓ Core executable built"

graphicals:		lib $(GRAPHICS_SO)
				@echo "✓ Graphical libraries built"

games:			lib $(GAMES_SO) $(MENU_SO)
				@echo "✓ Games and menu libraries built"

clean:
				$(RM) $(CORE_OBJ) $(MENU_OBJ) $(GRAPHICS_NCURSES_OBJ) $(GRAPHICS_SDL2_OBJ) $(GRAPHICS_SFML_OBJ) $(GAMES_MINESWEEPER_OBJ) $(GAMES_SNAKE_OBJ)
				@echo "✓ Objects cleaned"

fclean:			clean
				$(RM) $(ARCADE)
				$(RM) -r $(LIB_DIR)
				@echo "✓ Full clean completed"

re:				fclean all

reset:
				$(RM) *.sav
				@echo "✓ Save files cleaned"

.PHONY:			all core games graphicals lib clean fclean re reset
