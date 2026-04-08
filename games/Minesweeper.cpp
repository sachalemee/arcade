/*
** EPITECH PROJECT, 2026
** test
** File description:
** Minesweeper
*/

#include <cstdlib>
#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include "games/Minesweeper.hpp"

class Minesweeper : public IMinesweeper {
private:
    std::vector<std::vector<bool>> mines;
    std::vector<std::vector<CellState>> state;
    std::vector<std::vector<int>> count;
    int width;
    int height;
    int numMines;
    bool gameOver;
    bool won;
    bool initialized;
    Position cursor;

    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    bool hasEndTime;

    static constexpr int kTimeLimitSeconds = 300;

    static constexpr int kOffsetX = 2;
    static constexpr int kOffsetY = 2;


    Color getNumberColor(int num) const
    {
        switch (num) {
            case 1:
                return {0, 0, 255, 255};
            case 2:
                return {0, 128, 0, 255};
            case 3:
                return {255, 0, 0, 255};
            case 4:
                return {0, 0, 128, 255};
            case 5:
                return {128, 0, 0, 255};
            case 6:
                return {0, 128, 128, 255};
            case 7:
                return {0, 0, 0, 255};
            case 8:
                return {128, 128, 128, 255};
            default:
                return {0, 0, 0, 255};
        }
    }

public:
    Minesweeper() : width(16), height(16), numMines(40), gameOver(false), won(false), initialized(false), cursor({0, 0}) {
        srand(time(nullptr));
    }

    void init() override
    {
        init(width, height);
    }

    void update(Input input) override
    {
        updateTimer();
        if (gameOver || won)
            return;

        switch (input) {
            case Input::UP:
                setDirection(UP);
                break;
            case Input::DOWN:
                setDirection(DOWN);
                break;
            case Input::LEFT:
                setDirection(LEFT);
                break;
            case Input::RIGHT:
                setDirection(RIGHT);
                break;
            case Input::SELECT:
                revealCell(cursor.x, cursor.y);
                break;
            case Input::TOGGLE_SOUND:
                toggleFlag(cursor.x, cursor.y);
                break;
            case Input::RESTART:
                init(width, height);
                break;
            default:
                break;
        }
    }

    void stop() override
    {
        gameOver = true;
    }

    std::vector<Tile> getMap() const override
    {
        std::vector<Tile> tiles;
        tiles.reserve(static_cast<size_t>(width * height));
        const int offsetX = kOffsetX;
        const int offsetY = kOffsetY;
        const Color hudColor{255, 255, 255, 255};
        const Color hintColor{150, 150, 150, 255};
        const Color borderColor{180, 180, 180, 255};

        int flaggedCount = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (state[y][x] == FLAGGED)
                    flaggedCount++;
            }
        }

        auto end = hasEndTime ? endTime : std::chrono::steady_clock::now();
        int elapsedSeconds = static_cast<int>(
            std::chrono::duration_cast<std::chrono::seconds>(end - startTime).count());
        std::stringstream hud;
        hud << "Mines: " << (numMines - flaggedCount) << " | Time: " << elapsedSeconds << "s";
        tiles.push_back({offsetX, 0, TileType::TEXT, hudColor, hud.str()});
        tiles.push_back({offsetX, offsetY + height + 1, TileType::TEXT, hintColor,
            "ARROWS: Move | ENTER: Reveal | SPACE: Flag | R: Restart | E: Quit"});

        for (int x = 0; x < width; x++) {
            tiles.push_back({offsetX + x, offsetY - 1, TileType::EMPTY, borderColor, "-"});
            tiles.push_back({offsetX + x, offsetY + height, TileType::EMPTY, borderColor, "-"});
        }
        for (int y = 0; y < height; y++) {
            tiles.push_back({offsetX - 1, offsetY + y, TileType::EMPTY, borderColor, "|"});
            tiles.push_back({offsetX + width, offsetY + y, TileType::EMPTY, borderColor, "|"});
        }
        tiles.push_back({offsetX - 1, offsetY - 1, TileType::EMPTY, borderColor, "+"});
        tiles.push_back({offsetX + width, offsetY - 1, TileType::EMPTY, borderColor, "+"});
        tiles.push_back({offsetX - 1, offsetY + height, TileType::EMPTY, borderColor, "+"});
        tiles.push_back({offsetX + width, offsetY + height, TileType::EMPTY, borderColor, "+"});

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Tile tile;
                tile.x = offsetX + x;
                tile.y = offsetY + y;
                tile.text = "";

                if (state[y][x] == HIDDEN) {
                    tile.type = TileType::WALL;
                    tile.color = {192, 192, 192, 255};
                    tile.text = "-";
                } else if (state[y][x] == FLAGGED) {
                    tile.type = TileType::TEXT;
                    tile.color = {0, 100, 255, 255};
                    tile.text = "F";
                } else if (mines[y][x]) {
                    tile.type = TileType::TEXT;
                    tile.color = {255, 0, 0, 255};
                    tile.text = "*";
                } else {
                    int cellCount = count[y][x];
                    tile.type = TileType::WALL;
                    tile.color = {189, 189, 189, 255};
                    if (cellCount > 0) {
                        tile.type = TileType::TEXT;
                        tile.color = getNumberColor(cellCount);
                        tile.text = std::to_string(cellCount);
                    }
                }

                tiles.push_back(tile);
            }
        }

        int cursorX = offsetX + cursor.x;
        int cursorY = offsetY + cursor.y;
        tiles.push_back({cursorX, cursorY, TileType::EMPTY, {255, 255, 0, 255}, "O"});

        return tiles;
    }

    int getScore() const override
    {
        int revealed = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (state[y][x] == REVEALED && !mines[y][x]) {
                    revealed++;
                }
            }
        }
        if (won) {
            return revealed + getRemainingTimeSeconds();
        }
        return revealed;
    }

    bool isOver() const override
    {
        return gameOver;
    }

    void placeMines(int avoid_x, int avoid_y) 
    {
        for (auto& row : mines) {
            std::fill(row.begin(), row.end(), false);
        }
        int placed = 0;
        int x = 0;
        int y = 0;
        while (placed < numMines) {
            x = rand() % width;
            y = rand() % height;
            if (!mines[y][x] && (x != avoid_x || y != avoid_y)) {
                mines[y][x] = true;
                placed++;
            }
        }
    }

    void calculateMineCounts()
    {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (mines[y][x]) {
                    count[y][x] = -1;
                } else {
                    count[y][x] = countAdjacentMines(x, y);
                }
            }
        }
    }

    int countAdjacentMines(int x, int y)
    {
        int c = 0;
        int ny = 0;
        int nx = 0;

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                ny = y + dy;
                nx = x + dx;
                if (ny >= 0 && ny < height && nx >= 0 && nx < width && mines[ny][nx]) {
                    c++;
                }
            }
        }
        return c;
    }

    void initializeMines(int avoid_x, int avoid_y) 
    {
        if (initialized) 
            return;
        do {
            placeMines(avoid_x, avoid_y);
            calculateMineCounts();
        } while (count[avoid_y][avoid_x] != 0);
        initialized = true;
    }

    void init(int w, int h) override 
    {
        width = w;
        height = h;
        numMines = (w * h) / 6;
        
        mines.assign(h, std::vector<bool>(w, false));
        state.assign(h, std::vector<CellState>(w, HIDDEN));
        count.assign(h, std::vector<int>(w, 0));
        
        gameOver = false;
        won = false;
        initialized = false;
        hasEndTime = false;
        startTime = std::chrono::steady_clock::now();
        cursor = {0, 0};
    }

    void setDirection(Direction d) override 
    {
        if (gameOver || won)
            return;
        
        switch (d) {
            case UP:
                if (cursor.y > 0) cursor.y--;
                break;
            case DOWN:
                if (cursor.y < height - 1) cursor.y++;
                break;
            case LEFT:
                if (cursor.x > 0) cursor.x--;
                break;
            case RIGHT:
                if (cursor.x < width - 1) cursor.x++;
                break;
        }
    }

    void update() override
    {
        updateTimer();
        if (gameOver || won)
            return;
    }

    void floodFill(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;
        if (state[y][x] != HIDDEN)
            return;
        
        state[y][x] = REVEALED;
        
        if (count[y][x] == 0) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    floodFill(x + dx, y + dy);
                }
            }
        }
    }

    void revealAllMines()
    {
        gameOver = true;
        if (!hasEndTime) {
            endTime = std::chrono::steady_clock::now();
            hasEndTime = true;
        }
        for (int yy = 0; yy < height; yy++) {
            for (int xx = 0; xx < width; xx++) {
                if (mines[yy][xx]) {
                    state[yy][xx] = REVEALED;
                }
            }
        }
    }

    void revealCell(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;
        cursor = {x, y};
        if (state[y][x] != HIDDEN)
            return;
        
        if (!initialized) {
            initializeMines(x, y);
        }
        
        if (mines[y][x]) {
            revealAllMines();
        } else {
            floodFill(x, y);
            checkWin();
        }
    }

    void toggleFlag(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;
        cursor = {x, y};
        if (state[y][x] == REVEALED)
            return;
        
        if (state[y][x] == HIDDEN) {
            state[y][x] = FLAGGED;
        } else if (state[y][x] == FLAGGED) {
            state[y][x] = HIDDEN;
        }
    }

    void checkWin()
        {
        int revealed = 0;
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (state[y][x] == REVEALED && !mines[y][x]) {
                    revealed++;
                }
            }
        }
        
        int nonMines = width * height - numMines;
        if (revealed == nonMines) {
            won = true;
            if (!hasEndTime) {
                endTime = std::chrono::steady_clock::now();
                hasEndTime = true;
            }
        }
    }

    void updateTimer()
    {
        if (hasEndTime) {
            return;
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        if (elapsed >= kTimeLimitSeconds) {
            gameOver = true;
            endTime = now;
            hasEndTime = true;
        }
    }

    int getRemainingTimeSeconds() const
    {
        auto end = hasEndTime ? endTime : std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - startTime).count();
        int remaining = kTimeLimitSeconds - static_cast<int>(elapsed);
        return (remaining > 0) ? remaining : 0;
    }

    int getWidth() const override
    {
        return width;
    }

    int getHeight() const override
    {
        return height;
    }

    int getMineCount() const override
    {
        return numMines;
    }

    Position getCursor() const override
    {
        return cursor;
    }

    CellState getCellState(int x, int y) const override
    {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return state[y][x];
        }
        return HIDDEN;
    }

    bool isMine(int x, int y) const override
    {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return mines[y][x];
        }
        return false;
    }

    int getAdjacentMineCount(int x, int y) const override
    {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return count[y][x];
        }
        return 0;
    }

    std::vector<Position> getRevealedCells() const override
    {
        std::vector<Position> revealed;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (state[y][x] == REVEALED) {
                    revealed.push_back({x, y});
                }
            }
        }
        return revealed;
    }

    std::vector<Position> getFlaggedCells() const override {
        std::vector<Position> flagged;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (state[y][x] == FLAGGED) {
                    flagged.push_back({x, y});
                }
            }
        }
        return flagged;
    }

    std::vector<Position> getMineCells() const override {
        std::vector<Position> mines_pos;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (mines[y][x]) {
                    mines_pos.push_back({x, y});
                }
            }
        }
        return mines_pos;
    }

    bool isGameOver() const override {
        return gameOver;
    }

    bool isWon() const override {
        return won;
    }

    std::string getName() const override {
        return "Minesweeper";
    }
};

extern "C" IMinesweeper* createMinesweeper() {
    return new Minesweeper();
}

extern "C" IGame* createGame() {
    return new Minesweeper();
}
