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
#include "Menu.hpp"
#include "arcade.hpp"

Core::Core(const std::string &initialDisplayLib)
{
    mLibManager.scanDirectory(LIBDIRECTORY);
    loadScores();
    switchDisplay(initialDisplayLib);
    
    mGame = std::make_unique<Menu>();
    auto menu = dynamic_cast<Menu*>(mGame.get());
    if (menu) {
        menu->setGameLibs(mLibManager.getGameLibs());
        menu->setDisplayLibs(mLibManager.getDisplayLibs());
        std::vector<std::pair<std::string, int>> scores;
        for (const auto& player : mScores) {
            scores.push_back({player.name, player.highScore});
        }
        menu->setScores(scores);
        menu->init();
    }
    mInMenu = true;
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

void Core::updateMenuTransitions()
{
    auto menu = dynamic_cast<Menu*>(mGame.get());

    if (!menu || !menu->wantsToLaunchGame())
        return;
    std::string gameLib = menu->getSelectedGame();
    if (!gameLib.empty())
        switchGame(gameLib);
    if (menu->wantsToChangeDisplay()) {
        std::string displayLib = menu->getSelectedDisplay();
        if (!displayLib.empty()) {
            switchDisplay(displayLib);
        }
    }
}

void Core::updateDisplay()
{
    mDisplay->clear();
    mDisplay->draw(mGame->getMap());
    mDisplay->display();
}

void Core::updateGameLogic(Input input)
{
    if (!mGame)
        return;
    mGame->update(input);
    updateDisplay();
    if (mInMenu)
        updateMenuTransitions();
    else
        isGameEnded();
}

int Core::run()
{
    Input input = Input::NONE;

    while (mIsRunning) {
        input = mDisplay->getInput();
        handleInput(input);
        updateGameLogic(input);
    }
    return EXITSUCCESS;
}
