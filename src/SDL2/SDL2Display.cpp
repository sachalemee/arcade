/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SDL2Display
*/

#include <cctype>
#include "SDL2Display.hpp"
#include "IDisplay.hpp"
#include "arcade.hpp"

SDL_Color SDL2Display::toSDLColor(const Color &c) const
{
    return SDL_Color{c.r, c.g, c.b, c.a};
}

void SDL2Display::init(int width, int height, const std::string &title)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(EXITFAILURE);
    if (TTF_Init() < 0)
        exit(EXITFAILURE);
    SDL_StartTextInput();
    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!mWindow)
        exit(EXITFAILURE);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
        exit(EXITFAILURE);
    mFont = TTF_OpenFont(PATH_FONT.c_str(), CHAR_SIZE);
    if (!mFont)
        exit(EXITFAILURE);
}

Input SDL2Display::handleKey(SDL_Keycode keycode)
{
    switch (keycode) {
        case SDLK_UP:
            return Input::UP;
        case SDLK_DOWN:
            return Input::DOWN;
        case SDLK_LEFT:
            return Input::LEFT;
        case SDLK_RIGHT:
            return Input::RIGHT;
        case SDLK_n:
            return Input::NEXT_GAME;
        case SDLK_l:
            return Input::NEXT_LIB;
        case SDLK_r:
            return Input::RESTART;
        case SDLK_ESCAPE:
            return Input::MENU;
        case SDLK_e:
            return Input::EXIT;
        case SDLK_RETURN:
            return Input::SELECT;
        case SDLK_BACKSPACE:
            return Input::BACKSPACE;
        case SDLK_SPACE:
            return Input::TOGGLE_SOUND;
        default:
            return Input::NONE;
    }
}

void SDL2Display::handleTextInput(const SDL_Event &event)
{
    int i = 0;
    unsigned char ch = 0;

    for (i = 0; event.text.text[i] != '\0'; i++) {
        ch = (unsigned char)event.text.text[i];
        if (ch >= 32 && ch < 127)
            mTextInputs.push((char)ch);
    }
}

void SDL2Display::handleMouse(const SDL_Event &event)
{
    int button = 0;
    int x = 0;
    int y = 0;

    if (event.button.button == SDL_BUTTON_LEFT)
        button = 1;
    else if (event.button.button == SDL_BUTTON_RIGHT)
        button = 2;
    if (button == 0)
        return;
    x = event.button.x / SDL_TILE_SIZE;
    y = event.button.y / SDL_TILE_SIZE;
    mMouseClicks.push({x, y, button});
}

Input SDL2Display::getInput()
{
    SDL_Event event;
    Input result = Input::NONE;
    Input key;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return Input::EXIT;
        if (event.type == SDL_KEYDOWN) {
            key = handleKey(event.key.keysym.sym);
            if (key != Input::NONE && result == Input::NONE)
                result = key;
        }
        if (event.type == SDL_TEXTINPUT)
            handleTextInput(event);
        if (event.type == SDL_MOUSEBUTTONDOWN)
            handleMouse(event);
    }
    return result;
}

bool SDL2Display::getTextInput(char &ch)
{
    if (mTextInputs.empty())
        return false;
    ch = mTextInputs.front();
    mTextInputs.pop();
    return true;
}

bool SDL2Display::getMouseClick(int &x, int &y, int &button)
{
    MouseClick click;

    if (mMouseClicks.empty())
        return false;
    click = mMouseClicks.front();
    mMouseClicks.pop();
    x = click.x;
    y = click.y;
    button = click.button;
    return true;
}

void SDL2Display::drawText(const Tile &tile, const SDL_Color &c)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect dst;
    int w = 0;
    int h = 0;

    if (!mFont)
        return;
    surface = TTF_RenderText_Solid(mFont, tile.text.c_str(), c);
    if (!surface)
        return;
    texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
        return;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = tile.x * SDL_TILE_SIZE;
    dst.y = tile.y * SDL_TILE_SIZE;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(mRenderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void SDL2Display::drawRect(const Tile &tile, const SDL_Color &c)
{
    SDL_Rect rect;

    rect.x = tile.x * SDL_TILE_SIZE;
    rect.y = tile.y * SDL_TILE_SIZE;
    rect.w = SDL_TILE_SIZE - 1;
    rect.h = SDL_TILE_SIZE - 1;
    SDL_SetRenderDrawColor(mRenderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(mRenderer, &rect);
}

void SDL2Display::draw(const std::vector<Tile> &tiles)
{
    std::vector<Tile>::const_iterator it;
    SDL_Color c;

    for (it = tiles.begin(); it != tiles.end(); ++it) {
        c = toSDLColor(it->color);
        if (it->type == TileType::TEXT && mFont)
            drawText(*it, c);
        else
            drawRect(*it, c);
    }
}

void SDL2Display::display()
{
    SDL_RenderPresent(mRenderer);
}

void SDL2Display::clear()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
}

void SDL2Display::close()
{
    if (mFont)
        TTF_CloseFont(mFont);
    if (mRenderer)
        SDL_DestroyRenderer(mRenderer);
    if (mWindow)
        SDL_DestroyWindow(mWindow);
    SDL_StopTextInput();
    TTF_Quit();
    SDL_Quit();
}

extern "C" {
    IDisplay *createDisplay()
    {
        return new SDL2Display();
    }
}
