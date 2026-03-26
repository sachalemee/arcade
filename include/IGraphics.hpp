/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** IGraphics
*/

#ifndef IGRAPHICS_H
    #define IGRAPHICS_H

#include <cstdint>
#include <string>

struct Color {
    uint8_t r, g, b, a = 255;
};

class IGraphics {
    public:
        virtual ~IGraphics() = default;
        virtual void init(int width, int height, const std::string &title) = 0;
        virtual void close() = 0;
        virtual void clear() = 0;
        virtual void display() = 0;
        virtual void drawText(int x, int y, const std::string &str, Color c) = 0;
        virtual void drawRect(int x, int y, int w, int h, Color c) = 0;
        virtual void drawSprite(int x, int y, const std::string &key) {}
    protected:
    private:
};

#endif /* IGRAPHICS_H */
