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
    Input result = Input::NONE;

    switch (ch) {
        case KEY_UP: result = Input::UP; break;
        case KEY_DOWN: result = Input::DOWN; break;
        case KEY_LEFT: result = Input::LEFT; break;
        case KEY_RIGHT: result = Input::RIGHT; break;
        case 'n': result = Input::NEXT_GAME; break;
        case 'l': result = Input::NEXT_LIB; break;
        case 'r': result = Input::RESTART; break;
        case ESCKEY: result = Input::MENU; break;
        case 'e': result = Input::EXIT; break;
        case SPACEKEY: result = Input::SELECT; break;
        case KEY_BACKSPACE:
        case 127: result = Input::BACKSPACE; break;
        case ' ': result = Input::TOGGLE_SOUND; break;
    }
    if (ch >= 32 && ch <= 126)
        mTextInputs.push(static_cast<char>(ch));
    return result;
}

bool NCursesDisplay::getMouseClick(int &x, int &y, int &button)
{
    (void)x;
    (void)y;
    (void)button;
    return false;
}

bool NCursesDisplay::getTextInput(char &ch)
{
    if (mTextInputs.empty())
        return false;
    ch = mTextInputs.front();
    mTextInputs.pop();
    return true;
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
        if (!tile.text.empty() || tile.type == TileType::TEXT) {
            for (size_t i = 0; i < tile.text.length(); i++) {
                mvaddch(tile.y, tile.x + i, tile.text[i]);
            }
        } else {
            mvaddch(tile.y, tile.x, tileToChar(tile.type));
        }
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
    (void)width;
    (void)height;
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
