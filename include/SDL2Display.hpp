/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#ifndef SDL2DISPLAY_HPP
    #define SDL2DISPLAY_HPP

#include "IDisplay.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <queue>

const char* PATH_FONT = "./assets/MericaFuturistBlackItalic_PERSONAL_USE_ONLY.otf";
const int CHAR_SIZE = 14;
const int SDL_TILE_SIZE = 16;

class SDL2Display : public IDisplay {
    public:
        SDL2Display() = default;
        ~SDL2Display() = default;

        void init(int width, int height, const std::string &title) override;
        void close() override;
        void clear() override;
        void display() override;
        void draw(const std::vector<Tile> &tiles) override;

        Input getInput() override;
        bool getMouseClick(int &x, int &y, int &button) override;
        bool getTextInput(char &ch) override;

    private:
        SDL_Window* _window = nullptr;
        SDL_Renderer* _renderer = nullptr;
        TTF_Font* _font = nullptr;
        TTF_Font* _fallbackFont = nullptr;

        struct MouseClick {
            int x;
            int y;
            int button;
        };
        std::queue<MouseClick> _mouseClicks;
        std::queue<char> _textInputs;

        SDL_Color toSDLColor(const Color &c) const;
        void drawText(const Tile &tile, const SDL_Color &c);
        void drawRect(const Tile &tile, const SDL_Color &c);
};

#endif /* SDL2DISPLAY_HPP */
