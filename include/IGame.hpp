/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** IGame
*/

#ifndef IGAME_H
    #define IGAME_H

#include <vector>
#include "IGraphics.hpp"

enum class EventType {
    KEY_PRESSED,
    KEY_RELEASED,
    MOUSE_CLICK,
    WINDOW_CLOSE
};

enum class Key {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    ESCAPE,
    SPACE,
    NEXT_LIB,
    NEXT_GAME,
    RESTART,
    MENU,
    QUIT
};

struct Event {
    EventType type;
    Key key;
    int mouseX = 0;
    int mouseY = 0;
};

class IGame {
    public:
        virtual ~IGame() = default;
        virtual void init() = 0;
        virtual void update(const std::vector<Event>&) = 0;
        virtual void render(IGraphics&) = 0;
        virtual void stop() = 0;
        virtual int  getScore() const = 0;
        virtual bool isOver() const = 0;
        virtual std::string getName() const = 0;
    protected:
    private:
};

#endif /* IGAME_H */
