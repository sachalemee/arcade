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

    const std::string PATH_FONT = "./assets/PressStart2P-Regular.ttf";
    const int TILE_SIZE = 16;

class SFMLDisplay : public IDisplay {
    public:
        SFMLDisplay() = default;
        ~SFMLDisplay();

        void init(int width, int height, const std::string &title) override;
        void close() override;
        void clear() override;
        void display() override;
        void draw(const std::vector<Tile> &tiles) override;

        void drawTile(const Tile &tile);
        Input getInput() override;
        Input handleKey(sf::Keyboard::Key key);
        void handleMouse(const sf::Event &event);
        bool getMouseClick(int &x, int &y, int &button) override;
        bool getTextInput(char &ch) override;

    private:
        sf::RenderWindow *mWindow = nullptr;
        sf::Font mFont;
        sf::Color toSFMLColor(const Color &c) const;

        struct MouseClick {
            int x;
            int y;
            int button;
        };
        std::queue<MouseClick> mMouseClicks;
};

#endif /* SFMLDISPLAY_HPP */
