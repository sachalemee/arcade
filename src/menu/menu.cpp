/*
** EPITECH PROJECT, 2026
** G-OOP-400-PAR-4-1-arcade-45
** File description:
** menu
*/

#include "IMenu.hpp"

void Menu::init()
{
    _selectedGame = 0;
    _selectedDisplay = 0;
    _section = Section::GAMES;
    _playerName = "";
    _wantsLaunch = false;
    _wantsChangeDisplay = false;
}

void Menu::stop() {}

void setGameLibs(const std::vector<std::string>& libs)
{
    _gameLibs = libs;
}

void setDisplayLibs(const std::vector<std::string>& libs)
{
    _displayLibs = libs;
}

void setScores(const std::vector<std::pair<std::string, int>>& scores)
{
    _scores = scores;
}

std::string getSelectedGame() const
{
    return _gamesLibs[_selectedGame];
}

std::string getSelectedDisplay() const
{
    return _displayLibs[_selectedDisplkay];
}

std::string getPlayerName() const
{
    return _playerName;
}

bool wantsToLaunchGame() const
{
    return _wantsLaunch;
}

bool wantsToChangeDisplay() const
{
    return _WantsChangeDisplay;
}


