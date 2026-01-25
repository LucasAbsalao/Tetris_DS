#pragma once

#include <memory>
#include <random>

#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"

// Core game class: stores game state and applies Tetris rules
class Game {
private:
    std::unique_ptr<Grid> grid;        // Game board
    std::unique_ptr<Block> block;      // Current falling piece
    std::unique_ptr<Block> nextBlock;  // Next piece
    Stat stat;                         // Score / level

    int delayToGoDown;
    int normalSpeed;
    int fastSpeed;
    int actualSpeed;
    bool goDown;

    std::mt19937 rng;                  // RNG (seeded once)
    int lastId;                        // Avoid same piece twice in a row

private:
    int randomInt(int min, int max);
    bool collidesAt(const Block& b, int anchorRow, int anchorCol) const;

public:
    Game(int normalSpeed, int fastSpeed);

    std::unique_ptr<Block> generateBlock();

    void run();
    void update();
    void rotateBlock();
    void getMovement();

    bool CheckCollisionWall(char direction);
    bool checkCollisionFloor();
    bool checkCollisionFloor(int testRow);
    int getProjectionLine();

    // Read-only access
    const Grid& getGrid() const;
    const Block& getCurrentBlock() const;
    const Block& getNextBlock() const;
    const Stat& getStats() const;
};
