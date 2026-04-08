/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#ifndef SDL2DISPLAY_HPP
    #define SDL2DISPLAY_HPP

#include "IDisplay.hpp"

const std::string PATH_FONT = "./assets/MericaFuturistBlackItalic_PERSONAL_USE_ONLY.otf"

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

    private:
        SDL_Window* _windpw = nullptr;
        SDL_Renderer* _renderer = nullptr;
        TTF_Font* _font = nullptr;

        SDL_Color toSDLColor(const Color &c) const;
};

#endif /* SDL2DISPLAY_HPP */
