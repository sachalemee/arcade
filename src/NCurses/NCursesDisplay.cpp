/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** NCursesDisplay
*/

#include <curses.h>
#include <sys/types.h>
#include "IDisplay.hpp"
#include "arcade.hpp"
#include "NCursesDisplay.hpp"

int NCursesDisplay::toNCursesColor(u_int8_t channel)
{
    return static_cast<int>(channel) * 1000 / 255;
}

Input NCursesDisplay::getInput()
{
    int ch = getch();

    switch (ch) {
        case KEY_UP: return Input::UP;
        case KEY_DOWN: return Input::DOWN;
        case KEY_LEFT: return Input::LEFT;
        case KEY_RIGHT: return Input::RIGHT;
        case 'n': return Input::NEXT_GAME;
        case 'l': return Input::NEXT_LIB;
        case 'r': return Input::RESTART;
        case ESCKEY: return Input::MENU;
        case 'e': return Input::EXIT;
        case SPACEKEY: return Input::SELECT;
        case ' ': return Input::TOGGLE_SOUND;
    }
    return Input::NONE;
}

int NCursesDisplay::getColorPair(const Color &c)
{
    int key = (c.r << 16) | (c.g << 8) | (c.b << 4);
    auto it = mColorPairs.find(key);

    if (it != mColorPairs.end())
        return it->second;
    if (mNextPair >= COLOR_PAIRS)
        return EXITSUCCESS;
    init_color(mNextPair,
        toNCursesColor(c.r),
        toNCursesColor(c.g),
        toNCursesColor(c.b));
    init_pair(mNextPair, mNextPair, COLOR_BLACK);
    mColorPairs[key] = mNextPair;
    return mNextPair++;
}

int NCursesDisplay::tileToChar(TileType type) const
{
    switch (type) {
        case TileType::WALL: return '#';
        case TileType::PLAYER: return '@';
        case TileType::ENEMY: return 'X';
        case TileType::FOOD: return '*';
        case TileType::EMPTY: return ' ';
        default: return '?';
    }
}

void NCursesDisplay::draw(const std::vector<Tile> &tiles)
{
    int pair = 0;

    for (const auto &tile: tiles) {
        pair = getColorPair(tile.color);
        attron(COLOR_PAIR(pair));
        if (tile.type == TileType::TEXT) {
            for (size_t i = 0; i < tile.text.length(); i++) {
                mvaddch(tile.y, tile.x + i, tile.text[i]);
            }
        } else
            mvaddch(tile.y, tile.x, tileToChar(tile.type));
        attroff(COLOR_PAIR(pair));
    } 
}

void NCursesDisplay::display()
{
    refresh();
}

void NCursesDisplay::clear()
{
    ::erase();
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
