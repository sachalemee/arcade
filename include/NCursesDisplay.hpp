/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** NCursesDisplay
*/

#ifndef NCURSESDISPLAY_HPP
    #define NCURSESDISPLAY_HPP

#include "IDisplay.hpp"

class NCursesDisplay : public IDisplay {
    public:
        NCursesDisplay() = default;
        ~NCursesDisplay() = default;

        void init(int width, int height, const std::string &title) override;
        void close() override;
        void clear() override;
        void display() override;
        void draw(const std::vector<Tile> &tiles) override;

        Input getInput() override;

    private:
};

#endif /* NCURSESDISPLAY_HPP */
