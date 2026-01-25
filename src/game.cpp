#include "game.hpp"

// Constructor
Game::Game(int normalSpeed, int fastSpeed)
    : grid(std::make_unique<Grid>(10, 24, 25, (Vector2){125, 65}, 0)),
      stat(Stat()),
      normalSpeed(normalSpeed),
      fastSpeed(fastSpeed),
      actualSpeed(normalSpeed),
      goDown(false),
      delayToGoDown(0),
      rng(std::random_device{}()),
      lastId(-1)
{
    block = generateBlock();
    nextBlock = generateBlock();
}

// Random integer in [min, max]
int Game::randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// Generate a new piece (avoids same id twice)
std::unique_ptr<Block> Game::generateBlock() {
    int id;
    do {
        id = randomInt(1, 9); // 1–7 tetrominoes, 8–9 pentominoes
    } while (id == lastId);

    lastId = id;

    switch (id) {
        case 1: return std::make_unique<LBlock>();
        case 2: return std::make_unique<JBlock>();
        case 3: return std::make_unique<IBlock>();
        case 4: return std::make_unique<OBlock>();
        case 5: return std::make_unique<SBlock>();
        case 6: return std::make_unique<TBlock>();
        case 7: return std::make_unique<ZBlock>();
        case 8: return std::make_unique<PBlock>();
        case 9: return std::make_unique<UBlock>();
        default: return std::make_unique<TBlock>();
    }
}

// One game step
void Game::run() {
    getMovement();
    update();

    int count = 0;
    int line = grid->getCompletedLine(count);
    if (line != -1) {
        stat.update(count);
        grid->reallocateLines(line);
    }
}

// Gravity / locking
void Game::update() {
    if (delayToGoDown < actualSpeed) {
        delayToGoDown++;
    }
    else{
        if (checkCollisionFloor()) {
            grid->insertBlock(*block);
            std::swap(block, nextBlock);
            nextBlock = generateBlock();
        } else {
            block->moveDirection('D');
        }
        delayToGoDown = 0;
    }
    checkGameOver();
}

void Game::checkGameOver(){
    if(grid->isThereABlockInTheLine(0)){
        gameOver = true;
    }
}

// Input handling
void Game::getMovement() {
    if (IsKeyDown(KEY_DOWN) && !goDown) {
        actualSpeed = fastSpeed;
        goDown = true;
    } else if (IsKeyUp(KEY_DOWN) && goDown) {
        actualSpeed = normalSpeed;
        delayToGoDown = 0;
        goDown = false;
    }

    if (IsKeyPressed(KEY_RIGHT) && !CheckCollisionWall('R'))
        block->moveDirection('R');
    else if (IsKeyPressed(KEY_LEFT) && !CheckCollisionWall('L'))
        block->moveDirection('L');

    if (IsKeyPressed(KEY_UP))
        rotateBlock();
}

// Rotation with wall-kick + revert if invalid
void Game::rotateBlock() {
    int oldRot = block->getRotationState();
    int oldRow = block->getRow();
    int oldCol = block->getCol();

    block->rotate();

    const int kicks[][2] = {
        {0, 0}, {0, -1}, {0, 1}, {0, -2}, {0, 2}, {-1, 0}
    };

    for (auto& k : kicks) {
        int r = oldRow + k[0];
        int c = oldCol + k[1];

        if (!collidesAt(*block, r, c)) {
            block->move(r - block->getRow(), c - block->getCol());
            return;
        }
    }

    block->setRotationState(oldRot);
    block->move(oldRow - block->getRow(), oldCol - block->getCol());
}

// Collision helper
bool Game::collidesAt(const Block& b, int r, int c) const {
    for (const auto& p : b.getBlocks()) {
        int x = r + p.x;
        int y = c + p.y;

        if (y < 0 || y >= grid->getGridWidth()) return true;
        if (x >= grid->getGridHeight()) return true;
        if (x < 0) continue;

        if (grid->getColorXY(x, y) != grid->getBackgroundColor())
            return true;
    }
    return false;
}

// Wall collision
bool Game::CheckCollisionWall(char dir) {
    for (const auto& p : block->getBlocks()) {
        int x = block->getRow() + p.x;
        int y = block->getCol() + p.y;

        if (dir == 'R' && (y + 1 >= grid->getGridWidth() ||
            grid->getColorXY(x, y + 1) != grid->getBackgroundColor()))
            return true;

        if (dir == 'L' && (y - 1 < 0 ||
            grid->getColorXY(x, y - 1) != grid->getBackgroundColor()))
            return true;
    }
    return false;
}

// Floor collision
bool Game::checkCollisionFloor() {
    for (const auto& p : block->getBlocks()) {
        int x = block->getRow() + p.x;
        int y = block->getCol() + p.y;

        if (x + 1 >= grid->getGridHeight()) return true;
        if (grid->getColorXY(x + 1, y) != grid->getBackgroundColor())
            return true;
    }
    return false;
}

// Floor collision test at given row
bool Game::checkCollisionFloor(int testRow) {
    for (const auto& p : block->getBlocks()) {
        int x = testRow + p.x;
        int y = block->getCol() + p.y;

        if (x + 1 >= grid->getGridHeight()) return true;
        if (grid->getColorXY(x + 1, y) != grid->getBackgroundColor())
            return true;
    }
    return false;
}

// Ghost projection
int Game::getProjectionLine() {
    for (int r = block->getRow(); r < grid->getGridHeight(); r++) {
        if (checkCollisionFloor(r)) return r;
    }
    return -1;
}

Block Game::getBlock(){
    return *block;
}

bool Game::getGameOver(){
    return gameOver;
}
// Getters
const Grid& Game::getGrid() const { return *grid; }
const Block& Game::getCurrentBlock() const { return *block; }
const Block& Game::getNextBlock() const { return *nextBlock; }
const Stat& Game::getStats() const { return stat; }
