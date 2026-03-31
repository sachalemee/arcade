/*
** EPITECH PROJECT, 2026
** $
** File description:
** LibManager
*/

#ifndef LIBMANAGER_HPP
    #define LIBMANAGER_HPP

    #include <cstdlib>
    #include <filesystem>
    #include <string>
    #include <vector>
    #include <dlfcn.h>

    const std::string LIBDIRECTORY = "./lib/";
    const std::string GAMEPREFIX = "arcade_";
    const std::string GAMEEXTENSION = ".so";

class LibManager {
    public:
        LibManager() = default;
        ~LibManager() = default;

        bool loadAndIdentify(const std::string &libName);
        void scanDirectory(const std::string &dir);
        void fillLibVector(void *handle, void *sym, const std::filesystem::path &path);

        bool isGame() const { return isGameLib; }
        bool isDisplay() const { return isDisplayLib; }

        const std::vector<std::string> &getGameLibs() const { return gameLibs; }
        const std::vector<std::string> &getDisplayLibs() const { return displayLibs; }

    protected:
        bool isGameLib = false;
        bool isDisplayLib = false;
        std::vector<std::string> gameLibs;
        std::vector<std::string> displayLibs;
        std::string currentLib;
};

#endif /* LIBMANAGER_HPP */
