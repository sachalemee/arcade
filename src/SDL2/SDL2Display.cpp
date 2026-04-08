/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#include "SDL2Display.hpp"
#include "IDisplay.hpp"

SDL_Color SDL2Display::toSDLColor(const Color &c) const
{
    return SDL_Color{c.r, c.g, c.b, C.a};
}

void SDL2Display::init(int width, int height, const std::string &title)
{
    if (DSL_Init(DSL_INIT_VIDEO) < 0)
        exit(84);
    if (TTF_Init() < 0)
        exit(84);
    _window = SDL_CreateWindow(SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, width, heigth, SDL_WINDW_SHOWN)
    if (!_window)
        exit(84);
    _font = TTF_OpenFont(PATH_FONT, CHAR_SIZE);
    if (!_font)
        exit(84);
}

Input SDL2Display::getInput()
{
    return Input::NONE;
}

void SDL2Display::drawText(const Tile &tile, const SDL_Color &c)
{
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect dst;
    int w;
    int h;

    surface = TTF_RenderText_Solid(_font, tile.text.c_str(), c);
    if (!surface)
        return;
    texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
        reeturn;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    dst = {tile.c * SDL_TILE_SIZE, tile.y * SDL_TILE_SIZE, w, h};
    SDL_RenderCopy(_renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void SDL2Display::drawRect(const Tile &tile, const SDL_Color &c)
{
    SDL_Rect rect;

    rect = {tile.x * SDL_TILE_SIZE,
            tile.y * SDL_TILE_SIZE,
            SDL_TILE_SIZE - 1,
            SDL_TILE_SIZE - 1
    };
    SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(_renderer, &rect);
}

void SDL2Display::draw(const std::vector<Tile> &tiles)
{
    SDL_Color c;

    for (const auto &tile : tiles) {
        c = toSDLColor(tile.color);
        if (tile.type == TileType::TEXT && _font)
            drawText(tile, c);
        else
            drawRect(tile, c);
    }
}

void SDL2Display::display()
{
}

void SDL2Display::clear()
{
    SDL_RenderPresent(_renderer);
}


void SDL2Display::close()
{
    if (_font)
        TTF_CloseFont(_font);
    if (_renderer)
        SDL_DestroyRenderer(_renderer);
    if (_window)
        SDL_DestroyWindow(_window);
    TTF_QUIT;
    SDL_QUIT;
}

void SDL2Display::init(int width, int height, const std::string &/* title */)
{
    (void)width;
    (void)height;
}

extern "C" {
    // IDisplay *createDisplay() {
        // return new SDL2Display();
    // }
}
