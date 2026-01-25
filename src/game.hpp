#pragma once

#include <memory>
#include <random>
#include <raylib.h>
#include <utility>

#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"
#include "tetrominoes.hpp"

// Main game logic (state + rules)
class Game {
private:
    std::unique_ptr<Grid> grid;
    std::unique_ptr<Block> block;
    std::unique_ptr<Block> nextBlock;
    Stat stat;

    int delayToGoDown;
    int normalSpeed;
    int fastSpeed;
    int actualSpeed;
    bool goDown;
    bool gameOver;

    std::mt19937 rng;
    int lastId;

    int randomInt(int min, int max);
    bool collidesAt(const Block& b, int anchorRow, int anchorCol) const;

public:
    Game(int normalSpeed, int fastSpeed);

    std::unique_ptr<Block> generateBlock();

    void run();
    void update();
    void getMovement();
    void rotateBlock();

    bool CheckCollisionWall(char direction);
    bool checkCollisionFloor();
    bool checkCollisionFloor(int testRow);

    int getProjectionLine();

    void checkGameOver();
    bool getGameOver();

    Block getBlock();

    const Grid& getGrid() const;
    const Block& getCurrentBlock() const;
    const Block& getNextBlock() const;
    const Stat& getStats() const;
};
