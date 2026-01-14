#pragma once

#include <memory>
#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"

using namespace std;

// Core game class: stores game state and applies Tetris rules (no rendering)
class Game
{
private:
    unique_ptr<Grid> grid;        // Game board
    unique_ptr<Block> block;      // Current falling block
    unique_ptr<Block> nextBlock;  // Next block preview
    Stat stat;                    // Score/level/lines

    int delayToGoDown;            // Fall timer counter
    int normalSpeed;              // Normal fall speed
    int fastSpeed;                // Fast fall speed (soft drop)
    int actualSpeed;              // Current speed in use
    bool goDown;                  // Soft drop state flag

public:
    // Constructor with speed configuration (no Texture2D here anymore)
    Game(int normalSpeed, int fast_speed);

    unique_ptr<Block> generateBlock();
    int generateRandomNumber(int limit);

    void run();       // High-level step: input + update + line clear
    void update();    // Gravity / locking

    void rotateBlock();
    Block getBlock();

    void getMovement(); // Input-based movement (kept here by request)

    bool CheckCollisionWall(char direction);
    bool checkCollisionFloor();
    bool checkCollisionFloor(int positionX);

    int getProjectionLine();

    // Read-only access for rendering (no ownership transfer)
    const Grid& getGrid() const;
    const Block& getCurrentBlock() const;
    const Block& getNextBlock() const;
    const Stat& getStats() const;
};
