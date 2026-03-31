/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** main
*/

#include <cstring>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include "LibManager.hpp"

bool LibManager::loadAndIdentify(const std::string &libName)
{
    void *handle = dlopen(libName.c_str(), RTLD_NOW | RTLD_LOCAL);

    if (!handle) {
        std::cerr << "Error: " << dlerror() << "\n";
        return false;
    }
    dlerror();
    if (dlsym(handle, "createDisplay") && !dlerror()) {
        isDisplayLib = true;
    } else if (dlsym(handle, "createGame") && !dlerror()) {
        isGameLib = true;
    }
    dlclose(handle);
    currentLib = libName;
    return true;
}

void LibManager::scanDirectory(const std::string &dir)
{
}

bool LibManager::isGame() const
{
    return isGameLib;
}   

bool LibManager::isDisplay() const
{
    return isDisplayLib;
}

int helper(void)
{
    std::cout << "Usage:\n";
    std::cout << "\t./arcade [graphical_library]\n";
    std::cout << "Example:\n";
    std::cout << "\t./lib/arcade_ncurses.so\n";
    std::cout << "\t./lib/arcade_sfml.so\n";
    std::cout << "\t./lib/arcade_sdl2.so\n";
    return EXITFAILURE;
}

int error_handling(int ac, char **av, LibManager &libManager)
{
    std::filesystem::path libPath(av[1]);
 
    if (ac != 2)
        return helper();
    if (!libManager.isDisplay()) {
        std::cerr << "Error: '" << av[1] << "' not a graphical library\n";
        return EXITFAILURE;
    }
    if (!std::filesystem::exists(libPath)
        || strcmp(libPath.string().c_str(),
        GAMEEXTENSION.c_str()) != EXITSUCCESS)
        return EXITFAILURE;
    return EXITSUCCESS;
}

int main(int ac, char **av)
{
    LibManager libManager;

    if (error_handling(ac, av, libManager) == EXITFAILURE)
        return EXITFAILURE;
    return EXITSUCCESS;
}
