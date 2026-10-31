# Arcade - Documentation

## 1. Brief overview

Arcade is a gaming platform where users can pick a game and keep track of their scores. Graphics libraries and games are loaded dynamically at runtime using `dlopen`, `dlsym`, `dlclose`, and `dlerror`.

Important: the core program must never link directly to any graphics library. All graphics and game libraries work as plug-ins, and they're completely separated. Graphics can't touch game logic, and games have no rendering code.

## 2. Architecture

Main components:
- `Core.cpp`: runs the main loop, switches libraries, saves scores
- `IDisplay.hpp`: interface for graphics libraries
- `IGame.hpp`: interface for games
- `DLLoader.hpp`: wraps dlopen/dlsym/dlclose

All libraries end up as `.so` files in `./lib/`

## 3. Build

```bash
make              # Builds core + graphicals + games
make core         # Core only
make graphicals   # Graphics libraries only
make games        # Game libraries only
```

All `.so` files go to `./lib/`

## 4. Usage

```bash
./arcade ./lib/arcade_ncurses.so
```

You pass one argument: the graphics library to start with. It has to exist and be valid, otherwise the program exits with code **84**.

When it starts, the menu shows:
- Available games from `./lib/`
- Available graphics libraries from `./lib/`
- Player scores
- A field to enter your name

Keys:
- `L`: switch graphics library
- `N`: switch game
- `R`: restart the game
- `Esc`: go back to menu
- `E`: quit

If something goes wrong:
- Wrong number of arguments -> shows usage, exits **84**
- Invalid or non-graphics library -> shows error, exits **84**

---

## 5. Adding a Graphics Library

Create your library in `src/YourLib/YourLib.cpp` with a header file.

Here's what you need to do:

1. Inherit from `IDisplay` in [include/IDisplay.hpp](../include/IDisplay.hpp)
2. Implement the methods: `displayTile()`, `clear()`, `refresh()`, `getInput()`
3. Add a C factory function: `extern "C" { IDisplay *createDisplay() { ... } }`
4. Update the Makefile to build it with `-shared`
5. Output goes to `./lib/arcade_yourlib.so`

Check out [src/NCurses/NCursesDisplay.cpp](../src/NCurses/NCursesDisplay.cpp) for a complete example.

---

## 6. Adding a Game Library

Create your game in `games/YourGame.cpp` with a header file.

Here's what you need to do:

1. Inherit from `IGame` in [include/IGame.hpp](../include/IGame.hpp)
2. Implement: `update()`, `getTilesToDisplay()`, `isGameOver()`, `getScore()`, `getPlayerName()`
3. Add a C factory function: `extern "C" { IGame *createGame() { ... } }`
4. Update the Makefile to build it with `-shared`
5. Output goes to `./lib/arcade_yourgame.so`

See [games/Snake.cpp](../games/Snake.cpp) for a complete example.

---

## 7. Important Rules

- The core executable never links directly to game or graphics libraries
- All `.so` files must be treated the same way, no special cases
- Name all libraries as `arcade_<name>.so`
- Use the factory pattern: export `createDisplay` or `createGame` as C functions
- Use [include/DLLoader.hpp](../include/DLLoader.hpp) for safe dynamic loading