#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <dlfcn.h>
#include <memory>
#include <unistd.h>
#include "Core.hpp"
#include "DLLoader.hpp"
#include "IDisplay.hpp"
#include "IGame.hpp"
#include "IMenu.hpp"
#include "LibManager.hpp"
#include "arcade.hpp"

static Core *g_coreInstance = nullptr;

void signalHandler(int signal)
{
    if (signal == SIGINT && g_coreInstance)
        g_coreInstance->setRunning(false);
}

Core::Core(const std::string &initialDisplayLib)
{
    g_coreInstance = this;
    signal(SIGINT, signalHandler);
    mLibManager.scanDirectory(LIBDIRECTORY);
    loadScores();
    switchDisplay(initialDisplayLib);
    goToMenu();
}

void Core::goToMenu()
{
    if (mGame)
        mGame->stop();
    if (mMenu)
        mMenu->stop();
    mMenuLoader = std::make_unique<DLLoader<IMenu>>("./lib/arcade_menu.so");
    mMenu = mMenuLoader->getInstance("createMenu");
    mMenu->setGameLibs(mLibManager.getGameLibs());
    mMenu->setDisplayLibs(mLibManager.getDisplayLibs());
    std::vector<std::pair<std::string, int>> scores;
    for (const auto& player : mScores)
        scores.push_back({player.name, player.highScore});
    mMenu->setScores(scores);
    mMenu->init();
    mGame = nullptr;
    mGameLoader = nullptr;
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
    if (input == Input::EXIT) {
        mIsRunning = false;
        return;
    }
    if (input == Input::MENU) {
        goToMenu();
        return;
    }
    if (input == Input::RESTART && mGame) {
        mGame->init();
        return;
    }
}

void Core::switchDisplay(const std::string &path)
{
    if (mDisplay) {
        mDisplay->close();
        mDisplay = nullptr;
    }
    mDisplayLoader = nullptr;
    mDisplayLoader = std::make_unique<DLLoader<IDisplay>>(path);
    mDisplay = mDisplayLoader->getInstance("createDisplay");
    mDisplay->init(600, 800, "Arcade");
}

void Core::switchGame(const std::string &path)
{
    if (mGame)
        mGame->stop();
    mMenu = nullptr;
    mMenuLoader = nullptr;
    mGameLoader = std::make_unique<DLLoader<IGame>>(path);
    mGame = mGameLoader->getInstance("createGame");
    mGame->init();
    mInMenu = false;
}

void Core::isGameEnded()
{
    if (mGame && mGame->isOver()) {
        mPlayer.score = mGame->getScore();
        saveScore();
        goToMenu();
    }
}

void Core::updateMenuTransitions()
{
    if (!mMenu)
        return;
    if (mMenu->wantsToChangeDisplay()) {
        std::string displayLib = mMenu->getSelectedDisplay();
        if (!displayLib.empty())
            switchDisplay(displayLib);
    }
    if (mMenu->wantsToLaunchGame()) {
        std::string gameLib = mMenu->getSelectedGame();
        if (!gameLib.empty())
            switchGame(gameLib);
    }
}

void Core::updateDisplay()
{
    if (!mGame || !mDisplay)
        return;
    mDisplay->clear();
    mDisplay->draw(mGame->getMap());
    mDisplay->display();
}

void Core::updateGameLogic(Input input)
{
    if (mInMenu) {
        if (!mMenu) return;
        mMenu->update(input);
        mDisplay->clear();
        mDisplay->draw(mMenu->getMap());
        mDisplay->display();
        updateMenuTransitions();
    } else {
        if (!mGame) return;
        mGame->update(input);
        mDisplay->clear();
        mDisplay->draw(mGame->getMap());
        mDisplay->display();
        isGameEnded();
    }
}

int Core::run()
{
    Input input = Input::NONE;

    while (mIsRunning) {
        input = mDisplay->getInput();
        handleInput(input);
        updateGameLogic(input);
        usleep(16000);
    }
    return EXITSUCCESS;
}
