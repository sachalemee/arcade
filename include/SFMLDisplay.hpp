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

    private:
        sf::RenderWindow _window;
        sf::Font    _font;
        sf::Color toSFMLColor(const Color &c) const;
};

#endif /* SFMLDISPLAY_HPP */
