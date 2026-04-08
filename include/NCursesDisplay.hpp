/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** NCursesDisplay
*/

#ifndef NCURSESDISPLAY_HPP
    #define NCURSESDISPLAY_HPP

    #include <queue>
    #include <unordered_map>
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
        bool getMouseClick(int &x, int &y, int &button) override;
        bool getTextInput(char &ch) override;

    private:
        int mWidth;
        int mHeight;
        int mNextPair = 1;
        std::unordered_map<int, int> mColorPairs;
        std::queue<char> mTextInputs;

        int toNCursesColor(u_int8_t channel);
        int getColorPair(const Color &c);
        int tileToChar(TileType type) const;
};

#endif /* NCURSESDISPLAY_HPP */
