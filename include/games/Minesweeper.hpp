/*
** EPITECH PROJECT, 2026
** arcade - IMinesweeper
** File description:
** Minesweeper game interface
*/

#ifndef I_MINESWEEPER_HPP
#define I_MINESWEEPER_HPP

#include <vector>
#include <string>
#include "ICommon.hpp"
#include "IGame.hpp"

enum CellState {
    HIDDEN,
    REVEALED,
    FLAGGED
};

class IMinesweeper : public IGame {
public:
    virtual ~IMinesweeper() = default;

    // IGame interface - provide default implementations
    virtual void init() {}
    virtual void update(Input) {}
    virtual std::vector<Tile> getMap() const { return {}; }
    virtual int getScore() const { return 0; }
    virtual bool isOver() const { return false; }
    virtual std::string getName() const { return ""; }

    // Minesweeper-specific methods (actual implementation)
    virtual void init(int width, int height) = 0;
    virtual void setDirection(Direction d) = 0;
    virtual void revealCell(int x, int y) = 0;
    virtual void toggleFlag(int x, int y) = 0;
    virtual void update() = 0;
    virtual bool isGameOver() const = 0;
    virtual bool isWon() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getMineCount() const = 0;
    virtual Position getCursor() const = 0;
    virtual CellState getCellState(int x, int y) const = 0;
    virtual bool isMine(int x, int y) const = 0;
    virtual int getAdjacentMineCount(int x, int y) const = 0;
    virtual std::vector<Position> getRevealedCells() const = 0;
    virtual std::vector<Position> getFlaggedCells() const = 0;
    virtual std::vector<Position> getMineCells() const = 0;
};

#endif // I_MINESWEEPER_HPP
