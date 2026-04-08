/*
** EPITECH PROJECT, 2026
** test
** File description:
** Snake
*/

#include <cstdlib>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include "games/SnakeGame.hpp"

class Snake : public ISnakeGame {
private:
    std::vector<Position> snake;
    Position food;
    int width;
    int height;
    bool gameOver;
    Direction dir;
    Direction nextDir;
    int tickCounter;
    std::chrono::steady_clock::time_point startTime;
    int elapsedSeconds;

    static constexpr int kOffsetX = 2;
    static constexpr int kOffsetY = 2;
    static constexpr int kTickDelay = 6;

    bool isOnSnake(int x, int y) const
    {
        for (const auto& segment : snake) {
            if (segment.x == x && segment.y == y) {
                return true;
            }
        }
        return false;
    }

    bool placeFood()
    {
        std::vector<Position> freeCells;
        freeCells.reserve(static_cast<size_t>(width * height));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (!isOnSnake(x, y)) {
                    freeCells.push_back({x, y});
                }
            }
        }

        if (freeCells.empty()) {
            return false;
        }

        food = freeCells[static_cast<size_t>(rand()) % freeCells.size()];
        return true;
    }

    void updateElapsedTime()
    {
        auto now = std::chrono::steady_clock::now();
        elapsedSeconds = static_cast<int>(
            std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count());
    }

public:
    Snake()
        : width(20), height(10), gameOver(false), dir(RIGHT), nextDir(RIGHT),
          tickCounter(0), startTime(std::chrono::steady_clock::now()), elapsedSeconds(0) {}

    void init() override
    {
        init(width, height);
    }

    void update(Input input) override
    {
        updateElapsedTime();
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
            case Input::RESTART:
                init();
                return;
            default:
                break;
        }

        tickCounter = (tickCounter + 1) % kTickDelay;
        if (tickCounter != 0)
            return;
        update();
    }

    void init(int w, int h) override
    {
        width = w;
        height = h;
        snake.clear();
        int startX = w / 2;
        int startY = h / 2;
        for (int i = 0; i < 4 && (startX - i) >= 0; i++) {
            snake.push_back({startX - i, startY});
        }
        gameOver = false;
        dir = RIGHT;
        nextDir = RIGHT;
        tickCounter = 0;
        startTime = std::chrono::steady_clock::now();
        elapsedSeconds = 0;

        if (!placeFood()) {
            gameOver = true;
        }
    }

    void setDirection(Direction d) override
    {
        if ((dir == UP && d == DOWN) || 
            (dir == DOWN && d == UP) ||
            (dir == LEFT && d == RIGHT) ||
            (dir == RIGHT && d == LEFT)) {
            return;
        }
        nextDir = d;
    }

    void update() override
    {
        if (gameOver)
            return;
        dir = nextDir;
        Position head = snake.front();
        
        if (dir == UP)
            head.y--;
        else if (dir == DOWN) 
            head.y++;
        else if (dir == LEFT) 
            head.x--;
        else if (dir == RIGHT)
            head.x++;
        
        if (head.x < 0 || head.x >= width || head.y < 0 || head.y >= height) {
            gameOver = true;
            return;
        }
        
        for (const auto& segment : snake) {
            if (head.x == segment.x && head.y == segment.y) {
                gameOver = true;
                return;
            }
        }

        snake.insert(snake.begin(), head);

        if (head.x == food.x && head.y == food.y) {
            if (!placeFood()) {
                gameOver = true;
            }
        } else {
            snake.pop_back();
        }
    }

    void stop() override
    {
        gameOver = true;
    }

    std::vector<Tile> getMap() const override
    {
        std::vector<Tile> tiles;
        const int offsetX = kOffsetX;
        const int offsetY = kOffsetY;
        const Color bgA{30, 30, 30, 255};
        const Color bgB{36, 36, 36, 255};
        const Color headColor{0, 255, 150, 255};
        const Color bodyColor{0, 200, 100, 255};
        const Color foodColor{255, 100, 100, 255};
        const Color hudColor{255, 255, 255, 255};
        const Color hintColor{150, 150, 150, 255};
        const Color borderColor{180, 180, 180, 255};

        std::stringstream hud;
        hud << "Score: " << getScore() << " | Length: " << snake.size() << " | Time: "
            << elapsedSeconds << "s";
        tiles.push_back({offsetX, 0, TileType::TEXT, hudColor, hud.str()});
        tiles.push_back({offsetX, offsetY + height + 1, TileType::TEXT, hintColor,
            "ARROWS: Move | R: Restart | E: Quit"});

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color cellColor = ((x + y) % 2 == 0) ? bgA : bgB;
                tiles.push_back({offsetX + x, offsetY + y, TileType::EMPTY, cellColor, ""});
            }
        }

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

        std::string bodySymbol = ">";
        if (dir == LEFT)
            bodySymbol = "<";
        else if (dir == UP)
            bodySymbol = "^";
        else if (dir == DOWN)
            bodySymbol = "v";

        if (!snake.empty()) {
            const Position& head = snake.front();
            tiles.push_back({offsetX + head.x, offsetY + head.y,
                TileType::PLAYER, headColor, "O"});
        }
        for (size_t i = 1; i < snake.size(); i++) {
            const Position& segment = snake[i];
            tiles.push_back({offsetX + segment.x, offsetY + segment.y,
                TileType::WALL, bodyColor, bodySymbol});
        }
        tiles.push_back({offsetX + food.x, offsetY + food.y,
            TileType::FOOD, foodColor, "#"});

        return tiles;
    }

    int getScore() const override
    {
        if (snake.size() <= 1)
            return 0;
        return static_cast<int>((snake.size() - 1) * 10);
    }

    bool isOver() const override
    {
        return gameOver;
    }

    std::vector<Position> getSnake() const override
    {
        return snake;
    }

    Position getFood() const override
    {
        return food;
    }

    bool isGameOver() const override
    {
        return gameOver;
    }

    std::string getName() const override
    {
        return "Snake";
    }

    Direction getDirection() const override
    {
        return dir;
    }
};

extern "C" ISnakeGame* createSnake() {
    return new Snake();
}

extern "C" IGame* createGame() {
    return new Snake();
}
