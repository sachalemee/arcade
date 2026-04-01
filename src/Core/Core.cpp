/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Core
*/

#include <cstdio>
#include <dlfcn.h>
#include <memory>
#include "Core.hpp"
#include "DLLoader.hpp"
#include "IDisplay.hpp"
#include "IGame.hpp"
#include "LibManager.hpp"
#include "arcade.hpp"

Core::Core(const std::string &initialDisplayLib)
{
    mLibManager.scanDirectory(LIBDIRECTORY);
    loadScores();
    switchDisplay(initialDisplayLib);
}

void Core::saveScore()
{
    // save in a text file
}

void Core::loadScores()
{
    // load from a text file
}

void Core::handleInput(Input input)
{
    const auto &displayLibs = mLibManager.getDisplayLibs();
    const auto &gameLibs = mLibManager.getGameLibs();

    if (input == Input::EXIT) {
        mIsRunning = false;
        return;
    }
    if (input == Input::MENU) {
        mInMenu = true;
        mGame.reset();
        mGameLoader.reset();
        return;
    }
    if (input == Input::RESTART && mGame) {
        mGame->init();
        return;
    }
}

void Core::switchDisplay(const std::string &path)
{
    if (mDisplay)
        mDisplay->close();
    mDisplayLoader = std::make_unique<DLLoader<IDisplay>>(path);
    mDisplay = mDisplayLoader->getInstance("createDisplay");
    mDisplay->init(600, 800, "Arcade");
}

void Core::switchGame(const std::string &path)
{
    if (mGame)
        mGame->stop();
    mGameLoader = std::make_unique<DLLoader<IGame>>(path);
    mGame = mGameLoader->getInstance("createGame");
    mGame->init();
    mInMenu = false;
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
