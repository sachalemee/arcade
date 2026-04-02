/*
** EPITECH PROJECT, 2026
** G-OOP-400-PAR-4-1-arcade-45
** File description:
** IMenu
*/

#ifndef IMENU_HPP
#define IMENU_HPP

#include "IGame.hpp"
#include <string>
#include <vector>

class IMenu : public IGame {
public:
    virtual ~IMenu() = default;

    virtual void setGameLibs(const std::vector<std::string>& libs) = 0;
    virtual void setDisplayLibs(const std::vector<std::string>& libs) = 0;
    virtual void setScores(const std::vector<std::pair<std::string, int>>& scores) = 0;

    virtual std::string getSelectedGame() const = 0;
    virtual std::string getSelectedDisplay() const = 0;
    virtual std::string getPlayerName() const = 0;

    virtual bool wantsToLaunchGame() const = 0;
    virtual bool wantsToChangeDisplay() const = 0;
};

#endif