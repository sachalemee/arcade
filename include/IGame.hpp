/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** IGame
*/

#ifndef IGAME_HPP
    #define IGAME_HPP

    #include <vector>
    #include <string>
    #include "IDisplay.hpp"

class IGame {
    public:
        virtual ~IGame() = default;
        virtual void init() = 0;
        virtual void update(Input input) = 0;
        virtual std::vector<Tile> getMap() const = 0;
        virtual int getScore() const = 0;
        virtual bool isOver() const = 0;
        virtual void stop() = 0;
        virtual void onMouseClick(int /*x*/, int /*y*/, int /*button*/) {}
        virtual std::string getName() const = 0;
};

#endif /* IGAME_HPP */
