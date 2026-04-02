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
    int getScore() const override { return 0; }
    bool isOver() const override { return _wantsLaunch; }
    std::string getName() const override { return "Menu"; }

    // IMenu
    void setGameLibs(const std::vector<std::string>& libs) override;
    void setDisplayLibs(const std::vector<std::string>& libs) override;
    void setScores(const std::vector<std::pair<std::string, int>>& scores) override;
    std::string getSelectedGame() const override;
    std::string getSelectedDisplay() const override;
    std::string getPlayerName() const override;
    bool wantsToLaunchGame() const override;
    bool wantsToChangeDisplay() const override;

private:
    enum class Section { GAMES, DISPLAYS, NAME };

    std::vector<std::string> _gameLibs;
    std::vector<std::string> _displayLibs;
    std::vector<std::pair<std::string, int>> _scores;

    int _selectedGame    = 0;
    int _selectedDisplay = 0;
    bool _wantsLaunch        = false;
    bool _wantsChangeDisplay = false;
    std::string _playerName  = "";
    Section _section = Section::GAMES;
};

extern "C" IGame* createGame();

#endif /* MENU_HPP */