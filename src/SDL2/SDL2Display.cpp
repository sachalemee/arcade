/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#include "SDL2Display.hpp"
#include "IDisplay.hpp"

SDL_Color SDL2Display::toSDLColor(const Color &c) const
{
    return SDL_Color{c.r, c.g, c.b, C.a};
}

void SDL2Display::init(int width, int height, const std::string &title)
{
    if (DSL_Init(DSL_INIT_VIDEO) < 0)
        exit(84);
    if (TTF_Init() < 0)
        exit(84);
    _window = SDL_CreateWindow(SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, width, heigth, SDL_WINDW_SHOWN)
    if (!_window)
        exit(84);
    _font = TTF_OpenFont(PATH_FONT, 14);
    if (!_font)
        exit(84);
}

Input SDL2Display::getInput()
{
    return Input::NONE;
}

void SDL2Display::draw(const std::vector<Tile> &tiles)
{
    (void)tiles;
}

void SDL2Display::display()
{
}

void SDL2Display::clear()
{
}


void SDL2Display::close()
{
}

void SDL2Display::init(int width, int height, const std::string &/* title */)
{
    (void)width;
    (void)height;
}

extern "C" {
    // IDisplay *createDisplay() {
        // return new SDL2Display();
    // }
}
