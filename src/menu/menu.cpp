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

//SETTERS
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

// GETTERS
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

//MAP GETTERS
std::vector<Tile> Menu::getMap() const
{
    std::vector<Tile> tiles;
    int dispY = 0;
    int nameY = 0;

    addTitle(tiles);
    addGames(tiles);
    dispY = addDisplays(tiles);
    addScores(tiles);
    nameY = addPlayerName(tiles, dispY);
    addControls(tiles, nameY);
    return tiles;
}

void Meny::addTitle(std::vector<Tile> &tiles) const
{
    tiles.push_back({20, 0, TileType::TEXT,
                        {255, 255, 0, 255}, "=== ARCADE ==="});
}

void Menu::addGames(std::vector<Tile> &tiles) const
{
    std::string prefix;
    bool selected;
    Color c;

    tiles.push_back({0, 2, TileType::TEXT,
                        {255, 255, 0, 255},  "-- GAMES --"});
    for (int i = 0; i < (int)_gameLibs.size(); i++) {
        selected = (i ==_selectdGame && _section == Section::GAMES);
        c = selected ? Color{0, 255, 0, 255} : Color{255, 255, 255, 255};
        prefix = selected ? "> " : " ";
        tiles.push_back({0, 3 + i, TileType::TEXT, c, prefix + _gameLibs[i]});
    }    
}

int Menu::addDisplays(std::vector<Tile> &tiles) const
{
    std::string prefix;
    bool selected;
    Color c;

    tiles.push_back({0, 2, TileType::TEXT,
                        {255, 255, 0, 255},  "-- GAMES --"});
    for (int i = 0; i < (int)_displayLibs.size(); i++) {
        selected = (i ==_selectDisplay && _section == Section::DISPLAYS);
        c = selected ? Color{0, 255, 0, 255} : Color{255, 255, 255, 255};
        prefix = selected ? "> " : " ";
        tiles.push_back({0, 3 + i, TileType::TEXT, c, prefix + _displayLibs[i]});
    }    
}

void Menu::addScores(std::vector<Tiles> &tiles) const
{
    std::string line;

    tiles.push_back({40, 2, TileType::TEXT,
                        {255, 255, 0, 255}, "-- SCORES --"});
    for (int i = 0; i < (int)_scores.size(); i++) {
        line = _scores[i].first + " : " + std::to_string(_scores[i].second);
        tiles.push_back({40, 3 + i, TileType::TEXT,
                            {255, 255, 255, 255}, line});
    }
}

int Menu::addPlayerName(std::vector<Tile> &tiles, int dispY) const
{
    int nameY;
    Color nameColor;

    nameY = dispY + (int)_displayLibs.size() + 2;
    tiles.push_back({0, nameY, TileType::TEXT,
                        {255, 255, 0, 255}, "-- PLAYER NAME --"});
    nameColor = (_section == section::NAME)
                ? Color{0 ,255, 255, 255} : Color{255, 255, 255, 255};
    tiles.push_back({0, nameY + 1, TileType::TEXT,
                        nameColor, "> " + _playerName + "_"});
    return nameY;
}

void Menu::addControls(std::vector<Tile> &tiles, int nameY) const
{
    tiles.push_back({0, nameY + 3, TileType::TEXT,
                     {150, 150, 150, 255},
                     "UP/DOWN: navigate  LEFT/RIGHT: switch section  ENTER: launch"});
}

//UPDATE INPUT
void Menu::update(Input input)
{
    _wantsChangeDisplay = false;

    switch (input) {
        case Input::UP:           handleUp(); break;
        case Input::DOWN:         handleDown(); break;
        case Input::LEFT:
        case Input::RIGHT:        handleSectionSwitch(); break;
        case Input::NEXT_LIB:     handleNextLib(); break;
        case Input::NEXT_GAME:    handleNextGame(); break;
        case Input::SELECT:       handleSelect(); break;
        case Input::TOGGLE_SOUND: handleNameEdit(); break;
        default: break;
    }
}

void Menu::handleUp()
{
    if (_section == Section::GAMES && !_gameLibs.empty()) {
        _selectedGame = (_slectedGame - 1 
                        + _gameLibs.size()) % _gameLibs.size();
    } else if (_section == Section::DISPLAYS && !_displayLibs.empty()) {
        _selectedDisplay = (_selectedDisplay - 1 + _displayLibs.size())
                            % _displayLibs.size();
    }
}

void Menu::handleDown()
{
    if (_section == Section::GAMES && !_gameLibs.empty()) {
        _selectedGame = (_slectedGame + 1) % _gameLibs.size();
    } else if (_section == Section::DISPLAYS && !_displayLibs.empty()) {
        _selectedDisplay = (_selectedDisplay + 1) % _displayLibs.size();
    }
}

void Menu::hnadleSectionSwitch()
{
    if (_section == Section::GAMES)
        _section = Section::DISPLAYS;
    else if (_section == Section::DISPLAYS)
        _section = Section::NAME;
    else
        _section = Section::GAMES;
}

void Menu::handleNextLib()
{
    if (_displayLibs.empty())
        return;
    _selectedDisplay = (_selectedDisplay + 1) % _displayLibs.size();
    _wantsCahngeDisplay = true;
}

void Menu::handleNextGame()
{
    if (_gameLibs.empty())
        return;
    _selectedGame = (_selectedGame + 1) % _gameLibs.size();
}

void Menu::handleSelect()
{
    if ((_section == Section::NAME || _section == Section::GAMES) 
        && !_playerName.empty()) {
            _wantsLaunch = true;
        }
}

void Menu::handleNameEdit()
{
    if (_section == Section::NAME && !_playerName.empty())
        _playerName.pop_back();
}