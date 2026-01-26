#include "game.hpp"
// Constructor
Game::Game(int normalSpeed, int fastSpeed)
    : grid(std::make_unique<Grid>(10, 24, 25, (Vector2){125, 65}, 0)),
      stat(Stat()),
      originalSpeed(normalSpeed),
      normalSpeed(normalSpeed),
      fastSpeed(fastSpeed),
      actualSpeed(normalSpeed),
      goDown(false),
      delayToGoDown(0),
      position_x(125),
      position_y(65),
      gameOver(false),
      rng(std::random_device{}()),
      lastId(-1)
{
    block = generateBlock();
    nextBlock = generateBlock();
}

Game::Game(int normalSpeed, int fastSpeed, float position_x, float position_y):     grid(std::make_unique<Grid>(10,24,25,(Vector2){position_x,position_y},0)), 
                                                                                    block(generateBlock()), 
                                                                                    nextBlock(generateBlock()),
                                                                                    stat(Stat()),
                                                                                    position_x(position_x),
                                                                                    position_y(position_y),
                                                                                    originalSpeed (normalSpeed),
                                                                                    normalSpeed(normalSpeed),
                                                                                    fastSpeed(fastSpeed),
                                                                                    actualSpeed(normalSpeed),
                                                                                    goDown(false),
                                                                                    delayToGoDown(0),
                                                                                    gameOver(false)
{}

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

// Generate a new piece (avoids same id twice)
std::unique_ptr<Block> Game::generateSpecificBlock(int id) {
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
void Game::run(){
    if(!gameOver){
        getMovement();

        update();

        int count_lines = 0;
        int completed_line = grid->getCompletedLine(count_lines);
        if(completed_line!=-1)
            updateGridStat(count_lines, completed_line);
    }
}

void Game::updateGridStat(int count_lines, int completed_line){
    stat.update(count_lines);
    grid->reallocateLines(completed_line);
    changeSpeed(stat.getLevel());

}

void Game::changeSpeed(int level){
    int factor = (int)((originalSpeed - fastSpeed)/4);
    this->normalSpeed = (int)(originalSpeed - (factor*level));
    
    if (this->normalSpeed < fastSpeed) this->normalSpeed = fastSpeed; // Limit speed
    
    // Update actual speed if its not fastSpeed
    if (actualSpeed != fastSpeed) {
        actualSpeed = normalSpeed;
    }
}

void Game::update(){
    if(delayToGoDown<actualSpeed){
        delayToGoDown++;
        return;
    }
    else{
        if(checkCollisionFloor()){
            insertBlockInGrid();
        }
        else{
            moveBlock('D');
        }
        delayToGoDown=0;
    }
    checkGameOver();
}

void Game::insertBlockInGrid(){
    grid->insertBlock(*block);
    std::swap(block, nextBlock);
    nextBlock = generateBlock();
}

void Game::checkGameOver(){
    if(grid->isThereABlockInTheLine(0)){
        setGameOver();
    }
}

void Game::setGameOver(){
    this->gameOver = true;
}

void Game::getMovement(){
    if(IsKeyDown(KEY_DOWN) && !goDown){
        actualSpeed = fastSpeed;
        goDown = true;
    } else if (IsKeyUp(KEY_DOWN) && goDown) {
        actualSpeed = normalSpeed;
        delayToGoDown = 0;
        goDown = false;
    }
    if(IsKeyPressed(KEY_RIGHT) && !CheckCollisionWall('R')){
        moveBlock('R');
    }
    else if(IsKeyPressed(KEY_LEFT) && !CheckCollisionWall('L')){
        moveBlock('L');
    }
    if(IsKeyPressed(KEY_UP)){
        rotateBlock();
    }
}

// Rotation with wall-kick + revert if invalid
void Game::rotateBlock() {
    if (gameOver) return;

    int oldRot = block->getRotationState();
    int oldRow = block->getRow();
    int oldCol = block->getCol();

    block->rotate();

    const int kicks[][2] = {
        {0, 0}, {0, -1}, {0, 1}, {0, -2}, {0, 2}, {-1, 0}
    };

    for (const auto& k : kicks) {
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

void Game::moveBlock(char direction){
    block->moveDirection(direction);
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
bool Game::checkCollisionFloor(int testRow) const {
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
int Game::getProjectionLine() const{
    for (int r = block->getRow(); r < grid->getGridHeight(); r++) {
        if (checkCollisionFloor(r)) return r;
    }
    return -1;
}

void Game::receiveAttack(int lines){
    for(int i=0; i<lines;i++){
        int hole = randomInt(0,grid->getGridWidth()-1);
        grid->receiveAttack(hole);
    }
}

bool Game::getGameOver() const{
    return gameOver;
}

// Getters
const Grid& Game::getGrid() const { return *grid; }
const Block& Game::getCurrentBlock() const { return *block; }
const Block& Game::getNextBlock() const { return *nextBlock; }
const Stat& Game::getStats() const { return stat; }


Grid& Game::getGridRef(){
    return *grid;
}

Stat& Game::getStatRef(){
    return stat;
}

Block& Game::getBlockRef(){
    return *block;
}

Block* Game::getCurrentBlockPtr() {
    return block.get(); // return the pointer
}

Block& Game::getNextBlockRef(){
    return *nextBlock;
}

void Game::spawnBlock(int x, int y, int id, int rotationState){
    this->block = generateSpecificBlock(id);
    (this->block)->setRotationState(rotationState);
    (this->block)->setPosition(x,y);
    std::cout << "Block Created: " << this->block->getId() << std::endl;
}
