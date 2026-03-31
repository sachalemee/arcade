/*
** EPITECH PROJECT, 2026
** $
** File description:
** LibManager
*/

#ifndef LIBMANAGER_HPP
    #define LIBMANAGER_HPP

    #include <cstdlib>
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

        bool isGame() const;
        bool isDisplay() const;

        const std::vector<std::string> &getGameLibs() const;
        const std::vector<std::string> &getDisplayLibs() const;
    protected:
        bool isGameLib = false;
        bool isDisplayLib = false;
        std::vector<std::string> gameLibs;
        std::vector<std::string> displayLibs;
        std::string currentLib;
};

#endif /* LIBMANAGER_HPP */
