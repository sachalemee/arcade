/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Core
*/

#ifndef CORE_HPP
    #define CORE_HPP

    #include <memory>
    #include <string>
    #include <vector>
    #include "DLLoader.hpp"
    #include "IDisplay.hpp"
    #include "IGame.hpp"
    #include "LibManager.hpp"

struct Player {
    std::string name;
    int score = 0;
    int highScore = 0;
};

class Core {
    public:
        Core(const std::string &initialDisplayLib);
        ~Core() = default;
        int run();

    private:
        void switchDisplay(const std::string &path);
        void switchGame(const std::string &path);
        void handleInput(Input input);
        void saveScore();
        void loadScores();

        LibManager mLibManager;
        Player mPlayer;
        std::vector<Player> mScores;

        std::unique_ptr<DLLoader<IDisplay>> mDisplayLoader;
        std::unique_ptr<IDisplay> mDisplay;

        std::unique_ptr<DLLoader<IGame>> mGameLoader;
        std::unique_ptr<IGame> mGame;

        bool mIsRunning = true;
        bool mInMenu = true;
};

#endif /* CORE_HPP */
