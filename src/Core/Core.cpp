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

static Core *g_coreInstance = nullptr;

namespace {
    const char *kSaveFile = "./arcade.sav";

    void splitGameAndName(const std::string &value, std::string &game, std::string &name)
    {
        size_t sep = value.find(": ");
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
    for (const auto& player : mScores) {
        std::string label = player.name;
        if (!player.game.empty())
            label = player.game + ": " + player.name;
        scores.push_back({label, player.highScore});
    }
    mMenu->setScores(scores);
    if (!mLastSaveMessage.empty()) {
        mMenu->setStatusMessage(mLastSaveMessage);
        mLastSaveMessage.clear();
    } else {
        mMenu->setStatusMessage("");
    }
    mMenu->init();
    mGame = nullptr;
    mGameLoader = nullptr;
    mInMenu = true;
}

void Core::saveScore()
{
    if (mPlayer.name.empty())
        mPlayer.name = "Player";
    if (mPlayer.game.empty())
        mPlayer.game = "Unknown";
    bool found = false;
    for (auto &entry : mScores) {
        if (entry.name == mPlayer.name && entry.game == mPlayer.game) {
            entry.highScore = std::max(entry.highScore, mPlayer.score);
            found = true;
            break;
        }
    }
    if (!found) {
        Player entry;
        entry.game = mPlayer.game;
        entry.name = mPlayer.name;
        entry.highScore = mPlayer.score;
        mScores.push_back(entry);
    }

    std::ofstream out(kSaveFile, std::ios::trunc);
    if (!out)
        return;
    for (const auto &entry : mScores)
        out << entry.game << ": " << entry.name << "\t" << entry.highScore << "\n";
    mLastSaveMessage = "save OK";
}

void Core::loadScores()
{
    mScores.clear();
    std::ifstream in(kSaveFile);
    if (!in)
        return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        size_t tab = line.rfind('\t');
        if (tab == std::string::npos)
            continue;
        std::string left = line.substr(0, tab);
        std::string scoreStr = line.substr(tab + 1);
        int score = 0;
        std::istringstream scoreStream(scoreStr);
        scoreStream >> score;
        std::string game;
        std::string name;
        splitGameAndName(left, game, name);
        bool found = false;
        for (auto &entry : mScores) {
            if (entry.name == name && entry.game == game) {
                entry.highScore = std::max(entry.highScore, score);
                found = true;
                break;
            }
        }
        if (!found) {
            Player entry;
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
    if (mMenu) {
        mPlayer.name = mMenu->getPlayerName();
    }
    if (mPlayer.name.empty())
        mPlayer.name = "Player";
    mMenu = nullptr;
    mMenuLoader = nullptr;
    mGameLoader = std::make_unique<DLLoader<IGame>>(path);
    mGame = mGameLoader->getInstance("createGame");
    mPlayer.score = 0;
    mGame->init();
    mPlayer.game = mGame ? mGame->getName() : "Unknown";
    if (mPlayer.game.empty())
        mPlayer.game = "Unknown";
    mInMenu = false;
}

bool Core::isGameEnded()
{
    if (mGame && mGame->isOver()) {
        mPlayer.score = mGame->getScore();
        saveScore();
        goToMenu();
        return true;
    }
    return false;
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
        char ch = '\0';
        while (mDisplay->getTextInput(ch)) {
            mMenu->appendPlayerName(ch);
        }
        mDisplay->clear();
        mDisplay->draw(mMenu->getMap());
        mDisplay->display();
        updateMenuTransitions();
    } else {
        if (!mGame) return;
        int mouseX = 0;
        int mouseY = 0;
        int mouseButton = 0;
        while (mDisplay->getMouseClick(mouseX, mouseY, mouseButton)) {
            mGame->onMouseClick(mouseX, mouseY, mouseButton);
        }
        mGame->update(input);
        if (isGameEnded())
            return;
        mDisplay->clear();
        mDisplay->draw(mGame->getMap());
        mDisplay->display();
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
