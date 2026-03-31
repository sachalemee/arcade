/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** errorHandling
*/

#include <cstring>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include "LibManager.hpp"
#include "arcade.hpp"

static int helper(void)
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
    if (!std::filesystem::exists(libPath)) {
        std::cerr << "Error: '" << av[1] << "' not found\n";
        return EXITFAILURE;
    }
    if (libPath.extension() != GAMEEXTENSION) {
        std::cerr << "Error: '" << av[1] << "' not a .so file\n";
        return EXITFAILURE;
    }
    if (!libManager.loadAndIdentify(av[1])) {
        std::cerr << "Error: '" << av[1] << "' not a graphical library\n";
        return EXITFAILURE;
    }
    return EXITSUCCESS;
}
