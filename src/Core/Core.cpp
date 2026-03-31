/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Core
*/

#include <cstdio>
#include <dlfcn.h>
#include "Core.hpp"
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

int Core::run()
{
    while (mIsRunning) {
        printf("drole\n");
    }
    return EXITSUCCESS;
}
