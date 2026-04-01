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

        bool isGame() const { return mIsGameLib; }
        bool isDisplay() const { return mIsDisplayLib; }

        const std::vector<std::string> &getGameLibs() const { return mGameLibs; }
        const std::vector<std::string> &getDisplayLibs() const { return mDisplayLibs; }

    private:
        bool mIsGameLib = false;
        bool mIsDisplayLib = false;
        std::vector<std::string> mGameLibs;
        std::vector<std::string> mDisplayLibs;
        std::string mCurrentLib;
};

#endif /* LIBMANAGER_HPP */
