#include "game.hpp"
#include <raylib.h>
#include <random>
#include <ctime>

// Constructor: initializes the grid, blocks, stats, and falling speeds
Game::Game(int normalSpeed, int fastSpeed)
    : grid(make_unique<Grid>(10, 24, 25, (Vector2){125, 65}, 0)),
      block(generateBlock()),
      nextBlock(generateBlock()),
      stat(Stat()),
      normalSpeed(normalSpeed),
      fastSpeed(fastSpeed),
      actualSpeed(normalSpeed),
      goDown(false),
      delayToGoDown(0)
{}

// Creates a new random block near the top center of the grid
unique_ptr<Block> Game::generateBlock() {
    int position[2] = {0, grid->getGridWidth() / 2}; // spawn position (row, col)

    // Choose a random block type/index (renderer can map idx -> color later)
    int idx = generateRandomNumber(6) + 1; // example: 1..7 (depending on your block types)

    // Create and return the new block
    return make_unique<Block>(position, grid->getSize(), idx);
}

// Returns a random integer in [0, limit]
int Game::generateRandomNumber(int limit) {
    mt19937 gerador(std::time(0));
    uniform_int_distribution<int> distribuicao(0, limit);
    return distribuicao(gerador);
}

// High-level game step: input, update falling, clear lines, update stats
void Game::run() {
    getMovement();  // input (kept here as requested)
    update();       // gravity / locking

    int count_lines = 0;
    int completed_line = grid->getCompletedLine(count_lines);
    if (completed_line != -1) {
        stat.update(count_lines);
        grid->reallocateLines(completed_line);
    }
}

// Handles automatic falling using a frame counter (delayToGoDown)
void Game::update() {
    if (delayToGoDown < actualSpeed) {
        delayToGoDown++;
    } else {
        if (checkCollisionFloor()) {
            grid->insertBlock(getBlock()); // lock block into grid
            swap(block, nextBlock);        // current becomes next
            nextBlock = generateBlock();   // generate new next block
        } else {
            block->moveDirection('D');     // move down one step
        }
        delayToGoDown = 0;                 // reset fall timer
    }
}

// Reads keyboard input and applies movement/rotation (Raylib-dependent, kept here by request)
void Game::getMovement() {
    if (IsKeyDown(KEY_DOWN) && !goDown) {
        actualSpeed = fastSpeed;
        goDown = true;
    } else if (IsKeyUp(KEY_DOWN) && goDown) {
        actualSpeed = normalSpeed;
        delayToGoDown = 0;
        goDown = false;
    }

    if (IsKeyPressed(KEY_RIGHT) && !CheckCollisionWall('R')) {
        block->moveDirection('R');
    } else if (IsKeyPressed(KEY_LEFT) && !CheckCollisionWall('L')) {
        block->moveDirection('L');
    }

    if (IsKeyPressed(KEY_UP)) {
        rotateBlock();
    }
}

// Rotates the current block
void Game::rotateBlock() {
    block->rotate();
}

// Checks if moving left/right would collide with wall or occupied cells
bool Game::CheckCollisionWall(char direction) {
    for (int i = 0; i < block->getBlocks().size(); i++) {
        int posX = block->getPosition()[0] + block->getBlocks()[i].x;
        int posY = block->getPosition()[1] + block->getBlocks()[i].y;

        if (direction == 'R' &&
            (posY + 1 >= grid->getGridWidth() ||
             grid->getColorXY(posX, posY + 1) != grid->getBackgroundColor())) {
            return true;
        } else if (direction == 'L' &&
                   (posY - 1 < 0 ||
                    grid->getColorXY(posX, posY - 1) != grid->getBackgroundColor())) {
            return true;
        }
    }
    return false;
}

// Checks collision below the current block (bottom boundary or occupied cell)
bool Game::checkCollisionFloor() {
    for (int i = 0; i < block->getBlocks().size(); i++) {
        int posX = block->getPosition()[0] + block->getBlocks()[i].x;
        int posY = block->getPosition()[1] + block->getBlocks()[i].y;

        if (posX + 1 >= grid->getGridHeight() ||
            grid->getColorXY(posX + 1, posY) != grid->getBackgroundColor())
            return true;
    }
    return false;
}

// Same as above, but tests collision if the block were at a specific X (used for projection)
bool Game::checkCollisionFloor(int positionX) {
    for (int i = 0; i < block->getBlocks().size(); i++) {
        int posX = positionX + block->getBlocks()[i].x;
        int posY = block->getPosition()[1] + block->getBlocks()[i].y;

        if (posX + 1 >= grid->getGridHeight() ||
            grid->getColorXY(posX + 1, posY) != grid->getBackgroundColor())
            return true;
    }
    return false;
}

// Computes the landing row for the ghost piece by scanning downward
int Game::getProjectionLine() {
    for (int i = block->getPosition()[0]; i < grid->getGridHeight(); i++) {
        if (checkCollisionFloor(i)) {
            return i;
        }
    }
    return -1;
}

// Read-only accessors for game state (used by renderer)
const Grid& Game::getGrid() const { return *grid; }
const Block& Game::getCurrentBlock() const { return *block; }
const Block& Game::getNextBlock() const { return *nextBlock; }
const Stat& Game::getStats() const { return stat; }

// Returns the current block by value (copy)
Block Game::getBlock() {
    return *block;
}
