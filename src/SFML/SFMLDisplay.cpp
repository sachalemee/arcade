/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#include "IDisplay.hpp"
#include "SFMLDisplay.hpp"

sf::Color SFMLDisplay::toSFMLColor(const Color &c) const
{
    return sf::Color(c.r, c.g, c.b, c.a);
}

void SFMLDisplay::draw(const std::vector<Tile> &tiles)
{
    for (const auto &tile : tiles) {
        if (tile.type == TileType::TEXT) {
            sf::Text text;
            text.setFont(_font);
            text.setString(tile.text);
            text.setCharacterSize(CHAR_SIZE);
            text.setFillColor(toSFMLColor(tile.color));
            text.setPosition(tile.x * TILE_SIZE, tile.y * TILE_SIZE);
            _window.draw(text);
        } else {
            sf::RectangleShape rect(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE -1));
            rect.setPosition(tile.x * TILE_SIZE, tile.y * TILE_SIZE);
            rect.setFillColor(toSFMLColor(tile.color));
            _window.draw(rect);
        }
    }
}

void SFMLDisplay::display()
{
    _window.display();
}

void SFMLDisplay::clear()
{
    _window.clear(sf::Color::Black);
}


void SFMLDisplay::close()
{
    _window.close();
}

Input SFMLDisplay::getInput()
{
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            return Input::EXIT;
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Up:     return Input::UP;
                case sf::Keyboard::Down:   return Input::DOWN;
                case sf::Keyboard::Left:   return Input::LEFT;
                case sf::Keyboard::Right:  return Input::RIGHT;
                case sf::Keyboard::N:      return Input::NEXT_GAME;
                case sf::Keyboard::L:      return Input::NEXT_LIB;
                case sf::Keyboard::R:      return Input::RESTART;
                case sf::Keyboard::Escape: return Input::MENU;
                case sf::Keyboard::E:      return Input::EXIT;
                case sf::Keyboard::Enter:  return Input::SELECT;
                case sf::Keyboard::Space:  return Input::TOGGLE_SOUND;
                default: break;
            }
        }
    }
    return Input::NONE;
}

void SFMLDisplay::init(int width, int height, const std::string &title)
{
    _window.create(sf::VideoMode(width, height), title);
    _window.setFramerateLimit(FPS);

    if (!_font.loadFromFile(PATH_FONT))
        exit(84);
}

extern "C" {
    IDisplay *createDisplay() {
        return new SFMLDisplay();
    }
}
