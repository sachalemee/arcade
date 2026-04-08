/*
** EPITECH PROJECT, 2026
** arcade - ISnakeGame
** File description:
** Snake game interface
*/

#ifndef I_SNAKE_GAME_HPP
#define I_SNAKE_GAME_HPP

#include <vector>
#include <string>
#include "ICommon.hpp"
#include "IGame.hpp"

class ISnakeGame : public IGame {
public:
    virtual ~ISnakeGame() = default;

    // IGame interface - provide default implementations
    virtual void init() {}
    virtual void update(Input) {}
    virtual std::vector<Tile> getMap() const { return {}; }
    virtual int getScore() const { return 0; }
    virtual bool isOver() const { return false; }
    virtual std::string getName() const { return ""; }

    // Snake-specific methods (actual implementation)
    virtual void init(int width, int height) = 0;
    virtual void setDirection(Direction d) = 0;
    virtual void update() = 0;
    virtual std::vector<Position> getSnake() const = 0;
    virtual Position getFood() const = 0;
    virtual bool isGameOver() const = 0;
    virtual Direction getDirection() const = 0;
};

#endif // I_SNAKE_GAME_HPP
