/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Core
*/

#include <cstdio>
#include <dlfcn.h>
#include "Core.hpp"
#include "IDisplay.hpp"
#include "LibManager.hpp"
#include "arcade.hpp"

Core::Core(const std::string &initialDisplayLib)
{
    mLibManager.loadAndIdentify(LIBDIRECTORY);
    loadScores();
    switchDisplay(initialDisplayLib);
}

void Core::handleInput(Input input)
{
    return;
}

void Core::switchDisplay(const std::string &initialDisplayLib)
{
    return;
}

void Core::switchGame(const std::string &initialDisplayLib)
{
    mInMenu = false;
    return;
}

void Core::isGameEnded()
{
    if (mGame->isOver()) {
        mPlayer.score = mGame->getScore();
        saveScore();
        mInMenu = true;
        mGame.reset();
        mGameLoader.reset();
    }
}

int Core::run()
{
    Input input = Input::NONE;

    while (mIsRunning) {
        input = mDisplay->getInput();
        handleInput(input);
        if (mGame) {
            mGame->update(input);
            mDisplay->clear();
            mDisplay->draw(mGame->getMap());
            mDisplay->display();
            isGameEnded();
        }
    }
    return EXITSUCCESS;
}
