/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#ifndef SDL2DISPLAY_HPP
    #define SDL2DISPLAY_HPP

    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <queue>
    #include "IDisplay.hpp"

    const std::string PATH_FONT = "./assets/PressStart2P-Regular.ttf";
    const int CHAR_SIZE = 10;
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

        Input handleKey(SDL_Keycode keycode);
        void handleTextInput(const SDL_Event &event);
        void handleMouse(const SDL_Event &event);

    private:
        SDL_Window* mWindow = nullptr;
        SDL_Renderer* mRenderer = nullptr;
        TTF_Font* mFont = nullptr;

        struct MouseClick {
            int x;
            int y;
            int button;
        };
        std::queue<MouseClick> mMouseClicks;
        std::queue<char> mTextInputs;

        SDL_Color toSDLColor(const Color &c) const;
        void drawText(const Tile &tile, const SDL_Color &c);
        void drawRect(const Tile &tile, const SDL_Color &c);
};

#endif /* SDL2DISPLAY_HPP */
