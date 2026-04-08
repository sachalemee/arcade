/*
** EPITECH PROJECT, 2026
** G-OOP-400-PAR-4-1-arcade-45
** File description:
** menu
*/

#include "IMenu.hpp"
#include "Menu.hpp"

namespace {
    bool isMenuLib(const std::string &path)
    {
        return path.find("arcade_menu.so") != std::string::npos;
    }

    std::string formatLibName(const std::string &path)
    {
        size_t slash = path.find_last_of("/");
        std::string name = (slash == std::string::npos) ? path : path.substr(slash + 1);
        const std::string prefix = "arcade_";
        if (name.rfind(prefix, 0) == 0)
            name = name.substr(prefix.size());
        if (name.size() > 3 && name.substr(name.size() - 3) == ".so")
            name = name.substr(0, name.size() - 3);
        for (char &ch : name) {
            if (ch == '_')
                ch = ' ';
        }
        return name;
    }
}

void Menu::init()
{
    mSelectedGame = 0;
    mSelectedDisplay = 0;
    _section = Section::GAMES;
    mLayerName = "";
    mWantsLaunch = false;
    mWantsChangeDisplay = false;
}

void Menu::stop()
{
}

int Menu::getScore() const
{
    return 0;
}

bool Menu::isOver() const
{
    return mWantsLaunch;
}

std::string Menu::getName() const
{
    return "Menu";
}

void Menu::setGameLibs(const std::vector<std::string> &libs)
{
    mGameLibs.clear();
    for (const auto &lib : libs) {
        if (isMenuLib(lib))
            continue;
        mGameLibs.push_back(lib);
    }
    if (mSelectedGame >= (int)mGameLibs.size())
        mSelectedGame = 0;
}

void Menu::setDisplayLibs(const std::vector<std::string> &libs)
{
    mDisplayLibs = libs;
}

void Menu::setScores(const std::vector<std::pair<std::string, int>> &scores)
{
    mScores = scores;
}

std::string Menu::getSelectedGame() const
{
    if (mGameLibs.empty())
        return "";
    return mGameLibs[mSelectedGame];
}

std::string Menu::getSelectedDisplay() const
{
    if (mDisplayLibs.empty())
        return "";
    return mDisplayLibs[mSelectedDisplay];
}

std::string Menu::getPlayerName() const
{
    return mLayerName;
}

bool Menu::wantsToLaunchGame() const
{
    return mWantsLaunch;
}

bool Menu::wantsToChangeDisplay() const
{
    return mWantsChangeDisplay;
}

int Menu::addDisplays(std::vector<Tile> &tiles) const
{
    int dispY = 0;
    int i = 0;
    bool selected = false;
    Color c;
    std::string prefix;

    dispY = 3 + (int)mGameLibs.size() + 1;
    if (mGameLibs.empty())
        dispY = 3 + 1 + 1;
    tiles.push_back({0, dispY, TileType::TEXT,
            {255, 255, 0, 255}, "-- DISPLAYS --"});
    if (mDisplayLibs.empty()) {
        tiles.push_back({0, dispY + 1, TileType::TEXT,
            {200, 100, 100, 255}, "No displays available"});
    } else {
        for (i = 0; i < (int)mDisplayLibs.size(); i++) {
            selected = (i == mSelectedDisplay && _section == Section::DISPLAYS);
            c = selected ? Color{0, 255, 0, 255}
            : Color{255, 255, 255, 255};
            prefix = selected ? "> " : "  ";
            tiles.push_back({0, dispY + 1 + i, TileType::TEXT,
            c, prefix + mDisplayLibs[i]});
        }
    }
    return dispY;
}

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

void Menu::addTitle(std::vector<Tile> &tiles) const
{
    tiles.push_back({20, 0, TileType::TEXT,
        {255, 255, 0, 255}, "--- ARCADE ---"});
}

void Menu::addGames(std::vector<Tile> &tiles) const
{
    std::string prefix;
    bool selected;
    Color c;

    tiles.push_back({0, 2, TileType::TEXT,
                        {255, 255, 0, 255},  "-- GAMES --"});
    if (mGameLibs.empty()) {
        tiles.push_back({0, 3, TileType::TEXT,
                        {200, 100, 100, 255}, "No games available"});
    } else {
        for (int i = 0; i < (int)mGameLibs.size(); i++) {
            selected = (i == mSelectedGame && _section == Section::GAMES);
            c = selected ? Color{0, 255, 0, 255} : Color{255, 255, 255, 255};
            prefix = selected ? "> " : "  ";
            tiles.push_back({0, 3 + i, TileType::TEXT, c, prefix + formatLibName(mGameLibs[i])});
        }
    }    
}

void Menu::addScores(std::vector<Tile> &tiles) const
{
    std::string line;

    tiles.push_back({40, 2, TileType::TEXT,
        {255, 255, 0, 255}, "-- SCORES --"});
    for (int i = 0; i < (int)mScores.size(); i++) {
        line = mScores[i].first + " : " + std::to_string(mScores[i].second);
        tiles.push_back({40, 3 + i, TileType::TEXT,
                            {255, 255, 255, 255}, line});
    }
}

int Menu::addPlayerName(std::vector<Tile> &tiles, int dispY) const
{
    int nameY;
    Color nameColor;

    nameY = dispY + (int)mDisplayLibs.size() + 2;
    tiles.push_back({0, nameY, TileType::TEXT,
        {255, 255, 0, 255}, "-- PLAYER NAME --"});
    nameColor = (_section == Section::NAME)
        ? Color{0 ,255, 255, 255} : Color{255, 255, 255, 255};
    tiles.push_back({0, nameY + 1, TileType::TEXT,
                        nameColor, "> " + mLayerName + "_"});
    return nameY;
}

void Menu::addControls(std::vector<Tile> &tiles, int nameY) const
{
    tiles.push_back({0, nameY + 3, TileType::TEXT,
        {150, 150, 150, 255},
        "UP/DOWN: navigate  LEFT/RIGHT: switch section  ENTER: launch"});
}

void Menu::handleUp()
{
    if (_section == Section::GAMES &&!mGameLibs.empty()) {
        mSelectedGame = (mSelectedGame - 1
        + mGameLibs.size()) % mGameLibs.size();
    } else if (_section == Section::DISPLAYS &&!mDisplayLibs.empty()) {
        mSelectedDisplay = (mSelectedDisplay - 1 + mDisplayLibs.size())
        % mDisplayLibs.size();
    }
}

void Menu::handleDown()
{
    if (_section == Section::GAMES &&!mGameLibs.empty()) {
        mSelectedGame = (mSelectedGame + 1) % mGameLibs.size();
    } else if (_section == Section::DISPLAYS &&!mDisplayLibs.empty()) {
        mSelectedDisplay = (mSelectedDisplay + 1) % mDisplayLibs.size();
    }
}

void Menu::handleSectionSwitch()
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
    if (mDisplayLibs.empty())
        return;
    mSelectedDisplay = (mSelectedDisplay + 1) % mDisplayLibs.size();
    mWantsChangeDisplay = true;
}

void Menu::handleNextGame()
{
    if (mGameLibs.empty())
        return;
    mSelectedGame = (mSelectedGame + 1) % mGameLibs.size();
}

void Menu::handleSelect()
{
    if (_section == Section::DISPLAYS) {
        // On peut switcher de display avec SELECT
        mWantsChangeDisplay = true;
    } else if (_section == Section::NAME
        || (_section == Section::GAMES && !mGameLibs.empty())) {
        mWantsLaunch = true;
    }
}

void Menu::handleNameEdit()
{
    if (_section == Section::NAME &&!mLayerName.empty())
        mLayerName.pop_back();
}

void Menu::update(Input input)
{
    mWantsChangeDisplay = false;

    switch (input) {
        case Input::UP: handleUp(); break;
        case Input::DOWN: handleDown(); break;
        case Input::LEFT:
        case Input::RIGHT: handleSectionSwitch(); break;
        case Input::NEXT_LIB: handleNextLib(); break;
        case Input::NEXT_GAME: handleNextGame(); break;
        case Input::SELECT:
            if (mLayerName.empty())
                mLayerName = "Player";
            handleSelect();
            break;
        case Input::TOGGLE_SOUND: handleNameEdit(); break;
        default: break;
    }
}

extern "C" IGame *createGame()
{
    return new Menu();
}

extern "C" IMenu *createMenu()
{
    return new Menu();
}
