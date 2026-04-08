/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#ifndef SFMLDISPLAY_HPP
    #define SFMLDISPLAY_HPP

#include "IDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <queue>
#include <queue>

const std::string PATH_FONT = "./assets/MericaFuturistBlackItalic_PERSONAL_USE_ONLY.otf";
const int TILE_SIZE = 16;
const int FPS = 60;
const int CHAR_SIZE = 14;

class SFMLDisplay : public IDisplay {
    public:
        SFMLDisplay() = default;
        ~SFMLDisplay() = default;

        void init(int width, int height, const std::string &title) override;
        void close() override;
        void clear() override;
        void display() override;
        void draw(const std::vector<Tile> &tiles) override;

        Input getInput() override;
        bool getMouseClick(int &x, int &y, int &button) override;
        bool getTextInput(char &ch) override;

    private:
        sf::RenderWindow _window;
        sf::Font    _font;
        sf::Font    _fallbackFont;
        bool _hasPrimaryFont = false;
        bool _hasFallbackFont = false;
        struct MouseClick {
            int x;
            int y;
            int button;
        };

        sf::Color toSFMLColor(const Color &c) const;
        std::queue<MouseClick> _mouseClicks;
        std::queue<char> _textInputs;
};

#endif /* SFMLDISPLAY_HPP */
