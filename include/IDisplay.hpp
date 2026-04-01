/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** IGraphics
*/

#ifndef IDISPLAY_HPP
    #define IDISPLAY_HPP

    #include <string>
    #include <cstdint>
    #include <vector>

struct Color {
    uint8_t r, g, b, a = 255;
};

enum class Input {
    NONE,
    UP,                      // ↑
    DOWN,                    // ↓
    LEFT,                    // <-
    RIGHT,                   // ->
    NEXT_GAME,               // N
    NEXT_LIB,                // L
    RESTART,                 // R
    MENU,                    // Esc
    EXIT,                    // E
    SELECT,                  // Enter
    TOGGLE_SOUND             // Space
};

enum class TileType {
    EMPTY,
    WALL,
    PLAYER,
    ENEMY,
    FOOD,
    TEXT
};

struct Tile {
    int x;
    int y;
    TileType type;
    Color color;
    std::string text = "";
};

class IDisplay {
    public:
        virtual ~IDisplay() = default;

        virtual void init(int width, int height, const std::string &title) = 0;
        virtual void close() = 0;
        virtual void clear() = 0;
        virtual void display() = 0;
        virtual void draw(const std::vector<Tile> &tiles) = 0;
        virtual Input getInput() = 0;
};

#endif /* IDISPLAY_HPP */
