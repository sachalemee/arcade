/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** NCursesDisplay
*/

#include "NCursesDisplay.hpp"

void NCursesDisplay::init(int width, int height, const std::string &title)
{

}

void NCursesDisplay::close()
{

}

void NCursesDisplay::clear()
{

}

void NCursesDisplay::display()
{
}

void NCursesDisplay::draw(const std::vector<Tile> &tiles)
{
}

Input NCursesDisplay::getInput()
{
    return Input::NONE;
}

extern "C" {
    IDisplay *createDisplay() {
        return new NCursesDisplay();
    }
}
