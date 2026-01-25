#pragma once

#include <memory>
#include <random>
#include <raylib.h>
#include <utility>

#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"
#include "tetrominoes.hpp"

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
    bool gameOver;

    std::mt19937 rng;                  // RNG (seeded once)
    int lastId;                        // Avoid same piece twice in a row

    int randomInt(int min, int max);
    bool collidesAt(const Block& b, int anchorRow, int anchorCol) const;

    public:
        Game(int normalSpeed, int fast_speed, Texture2D background);
        Game(int normalSpeed, int fastSpeed);
        unique_ptr<Block> generateBlock();
        int generateRandomNumber(int limit);
        void draw();
        void drawUI();
        void run();
        void update();
        void rotateBlock();
        Block getBlock();
        void getMovement();
        bool CheckCollisionWall(char direction);
        bool checkCollisionFloor();
        bool checkCollisionFloor(int positionX);
        int getProjectionLine(); 
        void checkGameOver();

        
        bool getGameOver();

        const Grid& getGrid() const;
        const Block& getCurrentBlock() const;
        const Block& getNextBlock() const;
        const Stat& getStats() const;
};
