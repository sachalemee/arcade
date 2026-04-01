/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** NCursesDisplay
*/

#ifndef NCURSESDISPLAY_HPP
    #define NCURSESDISPLAY_HPP

#include "IDisplay.hpp"
#include <unordered_map>

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
        int mWidth;
        int mHeight;
        int mNextPair = 1;
        std::unordered_map<int, int> mColorPairs;

        int toNCursesColor(u_int8_t channel);
        int getColorPair(const Color &c);
};

#endif /* NCURSESDISPLAY_HPP */
