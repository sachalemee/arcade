/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Core
*/

#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <unistd.h>
#include "Core.hpp"
#include "DLLoader.hpp"
#include "IDisplay.hpp"
#include "IGame.hpp"
#include "IMenu.hpp"
#include "LibManager.hpp"
#include "arcade.hpp"

namespace {
    volatile sig_atomic_t gStopRunning = 0;
    const char *gameSavedFile = "./arcade.sav";

    void splitGameAndName(const std::string &value, std::string &game, std::string &name)
    {
        size_t sep;

        sep = value.find(": ");
        if (sep == std::string::npos) {
            game = "Unknown";
            name = value;
            return;
        }
        game = value.substr(0, sep);
        name = value.substr(sep + 2);
        if (game.empty())
            game = "Unknown";
        if (name.empty())
            name = "Player";
    }
}

void signalHandler(int signal)
{
    if (signal == SIGINT)
        gStopRunning = 1;
}

Core::Core(const std::string &initialDisplayLib)
{
    signal(SIGINT, signalHandler);
    mLibManager.scanDirectory(LIBDIRECTORY);
    loadScores();
    switchDisplay(initialDisplayLib);
    goToMenu();
}

void Core::goToMenu()
{
    std::vector<std::pair<std::string, int>> scores;
    std::string label = "";

    if (mGame)
        mGame->stop();
    if (mMenu)
        mMenu->stop();
    mMenuLoader = std::make_unique<DLLoader<IMenu>>("./lib/arcade_menu.so");
    mMenu = mMenuLoader->getInstance("createMenu");
    mMenu->setGameLibs(mLibManager.getGameLibs());
    mMenu->setDisplayLibs(mLibManager.getDisplayLibs());
    for (std::vector<Player>::const_iterator it = mScores.begin();
         it != mScores.end(); ++it) {
        label = it->name;
        if (!it->game.empty())
            label = it->game + ": " + it->name;
        scores.push_back(std::make_pair(label, it->highScore));
    }
    mMenu->setScores(scores);
    if (!mLastSaveMessage.empty()) {
        mMenu->setStatusMessage(mLastSaveMessage);
        mLastSaveMessage.clear();
    } else
        mMenu->setStatusMessage("");
    mMenu->init();
    mGame = nullptr;
    mGameLoader = nullptr;
    mInMenu = true;
}

void Core::saveScore()
{
    bool found = false;
    std::ofstream out;
    Player entry;

    if (mPlayer.name.empty())
        mPlayer.name = "Player";
    if (mPlayer.game.empty())
        mPlayer.game = "Unknown";
    found = false;
    for (std::vector<Player>::iterator it = mScores.begin();
         it != mScores.end(); ++it) {
        if (it->name == mPlayer.name && it->game == mPlayer.game) {
            it->highScore = std::max(it->highScore, mPlayer.score);
            found = true;
            break;
        }
    }
    if (!found) {
        entry.game = mPlayer.game;
        entry.name = mPlayer.name;
        entry.highScore = mPlayer.score;
        mScores.push_back(entry);
    }
    out.open(gameSavedFile, std::ios::trunc);
    if (!out)
        return;
    for (std::vector<Player>::const_iterator it = mScores.begin();
         it != mScores.end(); ++it) {
        out << it->game << ": " << it->name
            << "\t" << it->highScore << "\n";
    }
    mLastSaveMessage = "save OK";
}

void Core::loadScores()
{
    std::ifstream in;
    std::string line = "";
    size_t tab = 0;
    std::string left = "";
    std::string scoreStr = "";
    int score = 0;
    std::string game = "";
    std::string name = "";
    bool found = false;
    Player entry;

    mScores.clear();
    in.open(gameSavedFile);
    if (!in)
        return;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        tab = line.rfind('\t');
        if (tab == std::string::npos)
            continue;
        left = line.substr(0, tab);
        scoreStr = line.substr(tab + 1);
        std::istringstream(scoreStr) >> score;
        splitGameAndName(left, game, name);
        found = false;
        for (std::vector<Player>::iterator it = mScores.begin();
             it != mScores.end(); ++it) {
            if (it->name == name && it->game == game) {
                it->highScore = std::max(it->highScore, score);
                found = true;
                break;
            }
        }
        if (!found) {
            entry.game = game;
            entry.name = name;
            entry.highScore = score;
            mScores.push_back(entry);
        }
    }
}

void Core::handleInput(Input input)
{
    if (input == Input::EXIT) {
        if (mInMenu && mMenu && mMenu->isEditingName())
            return;
        mIsRunning = false;
        return;
    }
    if (input == Input::MENU) {
        goToMenu();
        return;
    }
    if (input == Input::RESTART && mGame)
        mGame->init();
    if (input == Input::NEXT_LIB) {
        const auto &displays = mLibManager.getDisplayLibs();
        if (!displays.empty()) {
            static int currentDisplay = 0;
            currentDisplay = (currentDisplay + 1) % displays.size();
            switchDisplay(displays[currentDisplay]);
        }
    }
    if (input == Input::NEXT_GAME && !mInMenu) {
        const auto &games = mLibManager.getGameLibs();
        if (!games.empty()) {
            static int currentGame = 0;
            currentGame = (currentGame + 1) % games.size();
            switchGame(games[currentGame]);
        }
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
    mDisplay->init(800, 600, "Arcade");
}

void Core::switchGame(const std::string &path)
{
    if (mGame)
        mGame->stop();
    if (mMenu)
        mPlayer.name = mMenu->getPlayerName();
    if (mPlayer.name.empty())
        mPlayer.name = "Player";
    mGame = nullptr;  // Important: nullify before destroying loader (dlclose)
    mGameLoader = nullptr;
    mMenu = nullptr;
    mMenuLoader = nullptr;
    mGameLoader = std::make_unique<DLLoader<IGame>>(path);
    mGame = mGameLoader->getInstance("createGame");
    mPlayer.score = 0;
    if (mGame)
        mGame->init();
    mPlayer.game = mGame ? mGame->getName() : "Unknown";
    if (mPlayer.game.empty())
        mPlayer.game = "Unknown";
    mInMenu = false;
}

bool Core::isGameEnded()
{
    if (!mGame)
        return false;
    if (!mGame->isOver())
        return false;
    mPlayer.score = mGame->getScore();
    saveScore();
    goToMenu();
    return true;
}

void Core::updateMenuTransitions()
{
    std::string displayLib = "";
    std::string gameLib = "";

    if (!mMenu)
        return;
    if (mMenu->wantsToChangeDisplay()) {
        displayLib = mMenu->getSelectedDisplay();
        if (!displayLib.empty())
            switchDisplay(displayLib);
    }
    if (mMenu->wantsToLaunchGame()) {
        gameLib = mMenu->getSelectedGame();
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
    char ch;
    int mouseX = 0;
    int mouseY = 0;
    int mouseButton = 0;

    if (mInMenu) {
        if (!mMenu)
            return;
        mMenu->update(input);
        while (mDisplay->getTextInput(ch))
            mMenu->appendPlayerName(ch);
        mDisplay->clear();
        mDisplay->draw(mMenu->getMap());
        mDisplay->display();
        updateMenuTransitions();
        return;
    }
    if (!mGame)
        return;
    while (mDisplay->getMouseClick(mouseX, mouseY, mouseButton))
        mGame->onMouseClick(mouseX, mouseY, mouseButton);
    mGame->update(input);
    if (isGameEnded())
        return;
    mDisplay->clear();
    mDisplay->draw(mGame->getMap());
    mDisplay->display();
}

int Core::run()
{
    Input input;

    input = Input::NONE;
    while (mIsRunning) {
        if (gStopRunning)
            mIsRunning = false;
        input = mDisplay->getInput();
        handleInput(input);
        updateGameLogic(input);
        usleep(16000);
    }
    return EXITSUCCESS;
}