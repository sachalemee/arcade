/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** NCursesDisplay
*/

#include <curses.h>
#include "NCursesDisplay.hpp"

Input NCursesDisplay::getInput()
{
    return Input::NONE;
}

void NCursesDisplay::draw(const std::vector<Tile> &tiles)
{
}

void NCursesDisplay::display()
{
    refresh();
}

void NCursesDisplay::clear()
{
    ::clear();
}


void NCursesDisplay::close()
{
    endwin();
}

void NCursesDisplay::init(int width, int height, const std::string &/* title */)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    if (has_colors()) {
        start_color();
        use_default_colors();
    }
}

extern "C" {
    IDisplay *createDisplay() {
        return new NCursesDisplay();
    }
}
