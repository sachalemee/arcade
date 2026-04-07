/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#include "SDL2Display.hpp"
#include "IDisplay.hpp"

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
