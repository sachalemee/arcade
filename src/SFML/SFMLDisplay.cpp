/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#include "IDisplay.hpp"
#include "arcade.hpp"
#include "SFMLDisplay.hpp"

SFMLDisplay::~SFMLDisplay()
{
    close();
}

sf::Color SFMLDisplay::toSFMLColor(const Color &c) const
{
    return sf::Color(c.r, c.g, c.b, c.a);
}

void SFMLDisplay::drawTile(const Tile &tile)
{
    sf::Text text;
    sf::RectangleShape rect(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));

    if (tile.type == TileType::TEXT) {
        text.setFont(mFont);
        text.setString(tile.text);
        text.setCharacterSize(10);
        text.setFillColor(toSFMLColor(tile.color));
        text.setPosition(tile.x * TILE_SIZE, tile.y * TILE_SIZE);
        mWindow->draw(text);
        return;
    }
    rect.setPosition(tile.x * TILE_SIZE, tile.y * TILE_SIZE);
    rect.setFillColor(toSFMLColor(tile.color));
    mWindow->draw(rect);
}

void SFMLDisplay::draw(const std::vector<Tile> &tiles)
{
    std::vector<Tile>::const_iterator it;

    if (!mWindow)
        return;
    for (it = tiles.begin(); it != tiles.end(); ++it)
        drawTile(*it);
}

void SFMLDisplay::display()
{
    if (!mWindow)
        return;
    mWindow->display();
}

void SFMLDisplay::clear()
{
    if (!mWindow)
        return;
    mWindow->clear(sf::Color::Black);
}

void SFMLDisplay::close()
{
    if (!mWindow)
        return;
    mWindow->close();
    delete mWindow;
    mWindow = nullptr;
}

Input SFMLDisplay::handleKey(sf::Keyboard::Key key)
{
    switch (key) {
        case sf::Keyboard::Up:
            return Input::UP;
        case sf::Keyboard::Down:
            return Input::DOWN;
        case sf::Keyboard::Left:
            return Input::LEFT;
        case sf::Keyboard::Right:
            return Input::RIGHT;
        case sf::Keyboard::N:
            return Input::NEXT_GAME;
        case sf::Keyboard::L:
            return Input::NEXT_LIB;
        case sf::Keyboard::R:
            return Input::RESTART;
        case sf::Keyboard::Escape:
            return Input::MENU;
        case sf::Keyboard::E:
            return Input::EXIT;
        case sf::Keyboard::Enter:
            return Input::SELECT;
        case sf::Keyboard::Space:
            return Input::TOGGLE_SOUND;
        default:
            return Input::NONE;
    }
}

Input SFMLDisplay::getInput()
{
    sf::Event event;

    if (!mWindow)
        return Input::NONE;
    while (mWindow->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            return Input::EXIT;
        if (event.type == sf::Event::KeyPressed)
            return handleKey(event.key.code);
    }
    return Input::NONE;
}

void SFMLDisplay::init(int width, int height, const std::string &title)
{
    close();
    mWindow = new sf::RenderWindow(sf::VideoMode(width, height), title);
    mWindow->setFramerateLimit(60);
    if (!mFont.loadFromFile(PATH_FONT))
        exit(EXITFAILURE);
}

bool SFMLDisplay::getMouseClick(int &x, int &y, int &button)
{
    (void)x;
    (void)y;
    (void)button;
    return false;
}

bool SFMLDisplay::getTextInput(char &ch)
{
    (void)ch;
    return false;
}

extern "C" {
    IDisplay *createDisplay()
    {
        return new SFMLDisplay();
    }
}