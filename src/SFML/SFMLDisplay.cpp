/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#include "IDisplay.hpp"
#include "SFMLDisplay.hpp"
#include <cctype>

namespace {
    const char *kFallbackFonts[] = {
        "/usr/share/fonts/adobe-source-code-pro-fonts/SourceCodePro-Regular.otf",
        "/usr/share/fonts/abattis-cantarell-fonts/Cantarell-Regular.otf",
        "/usr/share/fonts/aajohan-comfortaa-fonts/Comfortaa-Regular.otf"
    };

    bool needsFallbackFont(const std::string &text)
    {
        for (unsigned char ch : text) {
            if (std::isdigit(ch) || ch == '|')
                return true;
        }
        return false;
    }
}

sf::Color SFMLDisplay::toSFMLColor(const Color &c) const
{
    return sf::Color(c.r, c.g, c.b, c.a);
}

void SFMLDisplay::draw(const std::vector<Tile> &tiles)
{
    for (const auto &tile : tiles) {
        if (tile.type == TileType::TEXT) {
            sf::Text text;
            bool useFallback = needsFallbackFont(tile.text) || !_hasPrimaryFont;
            const sf::Font &font = (useFallback && _hasFallbackFont) ? _fallbackFont : _font;
            text.setFont(font);
            text.setString(tile.text);
            text.setCharacterSize(CHAR_SIZE);
            text.setFillColor(toSFMLColor(tile.color));
            text.setPosition(tile.x * TILE_SIZE, tile.y * TILE_SIZE);
            _window.draw(text);
        } else if (tile.type == TileType::EMPTY && !tile.text.empty()) {
            continue;
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
    Input result = Input::NONE;
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            result = Input::EXIT;
        if (event.type == sf::Event::KeyPressed) {
            Input key = Input::NONE;
            switch (event.key.code) {
                case sf::Keyboard::Up:     key = Input::UP; break;
                case sf::Keyboard::Down:   key = Input::DOWN; break;
                case sf::Keyboard::Left:   key = Input::LEFT; break;
                case sf::Keyboard::Right:  key = Input::RIGHT; break;
                case sf::Keyboard::N:      key = Input::NEXT_GAME; break;
                case sf::Keyboard::L:      key = Input::NEXT_LIB; break;
                case sf::Keyboard::R:      key = Input::RESTART; break;
                case sf::Keyboard::Escape: key = Input::MENU; break;
                case sf::Keyboard::E:      key = Input::EXIT; break;
                case sf::Keyboard::Enter:  key = Input::SELECT; break;
                case sf::Keyboard::BackSpace: key = Input::BACKSPACE; break;
                case sf::Keyboard::Space:  key = Input::TOGGLE_SOUND; break;
                default: break;
            }
            if (key != Input::NONE && result == Input::NONE)
                result = key;
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            int button = 0;
            if (event.mouseButton.button == sf::Mouse::Left)
                button = 1;
            else if (event.mouseButton.button == sf::Mouse::Right)
                button = 2;
            if (button != 0) {
                int x = event.mouseButton.x / TILE_SIZE;
                int y = event.mouseButton.y / TILE_SIZE;
                _mouseClicks.push({x, y, button});
            }
        }
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode >= 32 && event.text.unicode < 127) {
                _textInputs.push(static_cast<char>(event.text.unicode));
            }
        }
    }
    return result;
}

bool SFMLDisplay::getMouseClick(int &x, int &y, int &button)
{
    if (_mouseClicks.empty())
        return false;
    MouseClick click = _mouseClicks.front();
    _mouseClicks.pop();
    x = click.x;
    y = click.y;
    button = click.button;
    return true;
}

bool SFMLDisplay::getTextInput(char &ch)
{
    if (_textInputs.empty())
        return false;
    ch = _textInputs.front();
    _textInputs.pop();
    return true;
}

void SFMLDisplay::init(int width, int height, const std::string &title)
{
    _window.create(sf::VideoMode(width, height), title);
    _window.setFramerateLimit(FPS);

    _hasPrimaryFont = _font.loadFromFile(PATH_FONT);
    _hasFallbackFont = false;
    for (const char *path : kFallbackFonts) {
        if (_fallbackFont.loadFromFile(path)) {
            _hasFallbackFont = true;
            break;
        }
    }
    if (!_hasPrimaryFont && !_hasFallbackFont)
        exit(84);
}

extern "C" {
    IDisplay *createDisplay() {
        return new SFMLDisplay();
    }
}
