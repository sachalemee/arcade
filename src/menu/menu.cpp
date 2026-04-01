/*
** EPITECH PROJECT, 2026
** G-OOP-400-PAR-4-1-arcade-45
** File description:
** menu
*/

#include "IMenu.hpp"

void Menu::init()
{
    _selectedGame    = 0;
    _selectedDisplay = 0;
    _section         = Section::GAMES;
    _playerName      = "";
    _wantsLaunch     = false;
    _wantsChangeDisplay = false;
}

