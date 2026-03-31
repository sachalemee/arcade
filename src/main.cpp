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

    isGameLib = false;
    isDisplayLib = false;
    if (!handle) {
        std::cerr << "Error: " << dlerror() << "\n";
        return false;
    }
    dlerror();
    if (dlsym(handle, "createDisplay") && !dlerror())
        isDisplayLib = true;
    else if (dlsym(handle, "createGame") && !dlerror())
        isGameLib = true;
    dlclose(handle);
    currentLib = libName;
    return true;
}

void LibManager::fillLibVector(void *handle, void *sym, const std::filesystem::path &path)
{
    dlerror();
    sym = dlsym(handle, "createDisplay");
    if (sym && !dlerror())
        displayLibs.push_back(path.string());
    else {
        dlerror();
        sym = dlsym(handle, "createGame");
        if (sym && !dlerror())
            gameLibs.push_back(path.string());
    }
}

void LibManager::scanDirectory(const std::string &dir)
{
    std::filesystem::directory_iterator it(dir);
    std::filesystem::directory_iterator end;
    std::filesystem::directory_entry entry;
    std::filesystem::path path;
    void *handle = nullptr;
    void *sym = nullptr;

    for (; it != end; ++it) {
        entry = *it;
        path = entry.path();
        if (path.extension() != GAMEEXTENSION)
            continue;
        handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!handle)
            continue;
        fillLibVector(handle, sym, path);
        dlclose(handle);
    }
}

int main(int ac, char **av)
{
    LibManager libManager;

    if (error_handling(ac, av, libManager) == EXITFAILURE)
        return EXITFAILURE;
    return EXITSUCCESS;
}
