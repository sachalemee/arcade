/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Menu
*/
#ifndef MENU_HPP
#define MENU_HPP

#include "IMenu.hpp"

class Menu : public IMenu {
    public:
        // IGame
        void init() override;
        void stop() override;
        void update(Input input) override;
        std::vector<Tile> getMap() const override;
        int getScore() const override;
        bool isOver() const override;
        std::string getName() const override;
    
        // IMenu
        void setGameLibs(const std::vector<std::string> &libs) override;
        void setDisplayLibs(const std::vector<std::string> &libs) override;
        void setScores(const std::vector<std::pair<std::string, int>> &scores) override;
        std::string getSelectedGame() const override;
        std::string getSelectedDisplay() const override;
        std::string getPlayerName() const override;
        void appendPlayerName(char c) override;
        void setStatusMessage(const std::string &message) override;
        bool isEditingName() const override;
        bool wantsToLaunchGame() const override;
        bool wantsToChangeDisplay() const override;
    
    private:
        enum class Section { GAMES, DISPLAYS, NAME };
    
        std::vector<std::string> mGameLibs;
        std::vector<std::string> mDisplayLibs;
        std::vector<std::pair<std::string, int>> mScores;
    
        int mSelectedGame = 0;
        int mSelectedDisplay = 0;
        bool mWantsLaunch = false;
        bool mWantsChangeDisplay = false;
        std::string mLayerName = "";
        std::string mStatusMessage = "";

        int addPlayerName(std::vector<Tile> &tiles, int dispY) const;
        int addDisplays(std::vector<Tile> &tiles) const;
        void addTitle(std::vector<Tile> &tiles) const;
        void addGames(std::vector<Tile> &tiles) const;
        void addScores(std::vector<Tile> &tiles) const;
        void addControls(std::vector<Tile> &tiles, int nameY) const;
       
        void handleNextGame();
        void handleSelect();
        void handleNameEdit();

        void handleUp();
        void handleDown();
        void handleSectionSwitch();
        void handleNextLib();

        Section _section = Section::GAMES;
};

#endif /* MENU_HPP */
