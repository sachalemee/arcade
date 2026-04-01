/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#ifndef SDL2DISPLAY_HPP
    #define SDL2DISPLAY_HPP

#include "IDisplay.hpp"

class SDL2Display : public IDisplay {
    public:
        SDL2Display() = default;
        ~SDL2Display() = default;

        void init(int width, int height, const std::string &title) override;
        void close() override;
        void clear() override;
        void display() override;
        void draw(const std::vector<Tile> &tiles) override;

        Input getInput() override;

    private:
};

#endif /* SDL2DISPLAY_HPP */
