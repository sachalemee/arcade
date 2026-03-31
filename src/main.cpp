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
#include "arcade.hpp"

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
    std::filesystem::directory_iterator it(dir);
    std::filesystem::directory_iterator end;
    std::filesystem::directory_entry entry;
    std::filesystem::path path;
    void *handle;

    for (; it != end; ++it) {
        entry = *it;
        path = entry.path();
        if (path.extension() != GAMEEXTENSION)
            continue;
        handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!handle)
            continue;
        if (dlsym(handle, "createDisplay") && !dlerror())
            displayLibs.push_back(path.string());
        else if (dlsym(handle, "createGame") && !dlerror())
            gameLibs.push_back(path.string());
        dlclose(handle);
    }
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

int main(int ac, char **av)
{
    LibManager libManager;

    if (error_handling(ac, av, libManager) == EXITFAILURE)
        return EXITFAILURE;
    return EXITSUCCESS;
}
