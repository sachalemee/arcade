/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#include "IDisplay.hpp"
#include "SFMLDisplay.hpp"

void SFMLDisplay::init(int width, int height, const std::string &title)
{

}

void SFMLDisplay::close()
{

}

void SFMLDisplay::clear()
{

}

void SFMLDisplay::display()
{
}

void SFMLDisplay::draw(const std::vector<Tile> &tiles)
{
}

Input SFMLDisplay::getInput()
{
    return Input::NONE;
}

extern "C" {
    IDisplay *createDisplay() {
        return new SFMLDisplay();
    }
}
