/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SFMLDisplay
*/

#ifndef SFMLDISPLAY_HPP
    #define SFMLDISPLAY_HPP

#include "IDisplay.hpp"

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
};

#endif /* SFMLDISPLAY_HPP */
