/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#include "IDisplay.hpp"
#include "SFMLDisplay.hpp"

Input SFMLDisplay::getInput()
{
    return Input::NONE;
}

void SFMLDisplay::draw(const std::vector<Tile> &tiles)
{
}

void SFMLDisplay::display()
{
}

void SFMLDisplay::clear()
{
}


void SFMLDisplay::close()
{
}

void SFMLDisplay::init(int width, int height, const std::string &/* title */)
{
}


extern "C" {
    IDisplay *createDisplay() {
        return new SFMLDisplay();
    }
}
