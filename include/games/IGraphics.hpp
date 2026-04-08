/*
** EPITECH PROJECT, 2026
** arcade - IGraphics
** File description:
** Graphics interface
*/

#ifndef I_GRAPHICS_HPP
#define I_GRAPHICS_HPP

#include <string>
#include "IDisplay.hpp"

class IGraphics : public IDisplay {
public:
    virtual ~IGraphics() = default;

    virtual void init(int width, int height, const std::string& title) = 0;
    virtual void clear() = 0;
    virtual Input getInput() = 0;
    virtual void drawRect(int x, int y, int width, int height, const Color& color) = 0;
    virtual void drawText(const std::string& text, int x, int y, const Color& color, int size = 12) = 0;
    virtual void drawImage(const std::string& path, int x, int y, int width, int height) = 0;
    virtual void playSound(const std::string& file) = 0;
    virtual void display() = 0;
    virtual void close() = 0;
    virtual bool getMouseClick(int& x, int& y, int& button) = 0;
    virtual std::string getBackendType() const { return "unknown"; }
};

#endif // I_GRAPHICS_HPP
