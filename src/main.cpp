/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** main
*/

#include <cstring>
#include <filesystem>
#include <iostream>
#include "LibManager.hpp"

void LibManager::loadAndIdentify(const std::string &libName)
{
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
