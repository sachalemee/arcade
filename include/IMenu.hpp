/*
** EPITECH PROJECT, 2026
** G-OOP-400-PAR-4-1-arcade-45
** File description:
** IMenu
*/

/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** IMenu
*/
#ifndef IMENU_H
#define IMENU_H

#include "IGame.hpp"
#include <string>
#include <vector>

class IMenu : public IGame {
public:
    virtual ~IMenu() = default;

    // Alimenter le menu avec les libs disponibles
    virtual void setGameLibs(const std::vector<std::string>& libs) = 0;
    virtual void setGraphicLibs(const std::vector<std::string>& libs) = 0;

    // Récupérer les choix de l'utilisateur
    virtual std::string getSelectedGame() const = 0;
    virtual std::string getSelectedGraphic() const = 0;
    virtual std::string getPlayerName() const = 0;

    // Scores à afficher
    virtual void setScores(const std::vector<std::pair<std::string, int>>& scores) = 0;

    // Le menu signale au core quoi faire
    virtual bool wantsToLaunchGame() const = 0;
    virtual bool wantToChangeGraphic() const = 0;
};

#endif /* IMENU_H */