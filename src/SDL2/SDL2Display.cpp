/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#include "SDL2Display.hpp"
#include "IDisplay.hpp"
#include <cctype>

namespace {
    const char *kFallbackFonts[] = {
        "/usr/share/fonts/adobe-source-code-pro-fonts/SourceCodePro-Regular.otf",
        "/usr/share/fonts/abattis-cantarell-fonts/Cantarell-Regular.otf",
        "/usr/share/fonts/aajohan-comfortaa-fonts/Comfortaa-Regular.otf"
    };

    bool needsFallbackFont(const std::string &text)
    {
        for (unsigned char ch : text) {
            if (std::isdigit(ch) || ch == '|')
                return true;
        }
        return false;
    }
}

SDL_Color SDL2Display::toSDLColor(const Color &c) const
{
    return SDL_Color{c.r, c.g, c.b, c.a};
}

void SDL2Display::init(int width, int height, const std::string &title)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(84);
    if (TTF_Init() < 0)
        exit(84);
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!_window)
        exit(84);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        exit(84);
    _font = TTF_OpenFont(PATH_FONT, CHAR_SIZE);
    for (const char *path : kFallbackFonts) {
        _fallbackFont = TTF_OpenFont(path, CHAR_SIZE);
        if (_fallbackFont)
            break;
    }
    if (!_font && !_fallbackFont)
        exit(84);
}

Input SDL2Display::getInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return Input::EXIT;

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:     return Input::UP;
                case SDLK_DOWN:   return Input::DOWN;
                case SDLK_LEFT:   return Input::LEFT;
                case SDLK_RIGHT:  return Input::RIGHT;
                case SDLK_n:      return Input::NEXT_GAME;
                case SDLK_l:      return Input::NEXT_LIB;
                case SDLK_r:      return Input::RESTART;
                case SDLK_ESCAPE: return Input::MENU;
                case SDLK_e:      return Input::EXIT;
                case SDLK_RETURN: return Input::SELECT;
                case SDLK_SPACE:  return Input::TOGGLE_SOUND;
                default: break;
            }
        }
    }
    return Input::NONE;
}

void SDL2Display::drawText(const Tile &tile, const SDL_Color &c)
{
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    int w;
    int h;

    TTF_Font *font = _font;
    if ((needsFallbackFont(tile.text) || !font) && _fallbackFont)
        font = _fallbackFont;
    if (!font)
        return;
    surface = TTF_RenderText_Solid(font, tile.text.c_str(), c);
    if (!surface)
        return;
    texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
        return;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    SDL_Rect dst = {tile.x * SDL_TILE_SIZE, tile.y * SDL_TILE_SIZE, w, h};
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
    SDL_RenderPresent(_renderer);
}

void SDL2Display::clear()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}


void SDL2Display::close()
{
    if (_font)
        TTF_CloseFont(_font);
    if (_fallbackFont)
        TTF_CloseFont(_fallbackFont);
    if (_renderer)
        SDL_DestroyRenderer(_renderer);
    if (_window)
        SDL_DestroyWindow(_window);
    TTF_Quit();
    SDL_Quit();
}

extern "C" {
    IDisplay *createDisplay() {
        return new SDL2Display();
    }
}
