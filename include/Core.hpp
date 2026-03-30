/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Core
*/

#ifndef CORE_HPP
    #define CORE_HPP

#include <string>

class Core {
    public:
        ~Core() = default;
        int core();
    protected:
    private:
};

struct Player {
    std::string name;
    int score;
    int highScore;
};

#endif /* CORE_HPP */
