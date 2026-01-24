#include "game.hpp"

Game::Game(int normalSpeed, int fastSpeed, Texture2D background): grid(make_unique<Grid>(10,24,25,(Vector2){125,65},0)), 
                                                                   colors(Colors::getColors()), 
                                                                   block(generateBlock()), 
                                                                   nextBlock(generateBlock()),
                                                                   backgroundImg(background),
                                                                   stat(Stat()),
                                                                   position_x(125),
                                                                   position_y(65),
                                                                   normalSpeed(normalSpeed),
                                                                   fastSpeed(fastSpeed),
                                                                   actualSpeed(normalSpeed),
                                                                   goDown(false),
                                                                   delayToGoDown(0),
                                                                   gameOver(false)
{}

Game::Game(int normalSpeed, int fastSpeed, Texture2D background, float position_x, float position_y):   grid(make_unique<Grid>(10,24,25,(Vector2){position_x,position_y},0)), 
                                                                                                        colors(Colors::getColors()), 
                                                                                                        block(generateBlock()), 
                                                                                                        nextBlock(generateBlock()),
                                                                                                        backgroundImg(background),
                                                                                                        stat(Stat()),
                                                                                                        position_x(position_x),
                                                                                                        position_y(position_y),
                                                                                                        normalSpeed(normalSpeed),
                                                                                                        fastSpeed(fastSpeed),
                                                                                                        actualSpeed(normalSpeed),
                                                                                                        goDown(false),
                                                                                                        delayToGoDown(0),
                                                                                                        gameOver(false)
{}

void Game::draw(){
    // DrawTextureEx(backgroundImg, (Vector2){0.0,0.0}, 0.0f,1.0,WHITE);
    // DrawTextureEx(backgroundImg, (Vector2){(float)backgroundImg.width,0.0}, 0.0f,1.0,WHITE);

    grid->draw();

    drawUI();

    if(block!=nullptr){ //TODO: DrawBlocks
        block->draw(grid->getPosition());
        int projection_line = getProjectionLine();
        if(projection_line!=-1) block->drawProjection(grid->getPosition(), projection_line);
    }
}

void Game::drawUI(){ //TODO: Automatizar os valores de score e level para eles ficarem centralizados
    char str_score[50];
    DrawText("TETRIS", position_x+20, position_y-55, 50, {255,255,255,255});

    DrawText("SCORE", position_x+330, position_y-15, 30, {255,255,255,255});
    DrawRectangle(position_x+275,position_y+10,200,90,colors[0]);
    DrawText(stat.strScore().c_str(), position_x+365, position_y+25, 60, {255,255,255,255});

    DrawText("NEXT BLOCK", position_x+275, position_y+115, 30, {255,255,255,255});
    DrawRectangle(position_x+275, position_y+140,200,90,colors[0]);

    DrawText("LEVEL", position_x+330, position_y+485, 30, {255,255,255,255});
    DrawRectangle(position_x+275,position_y+510,200,90,colors[0]);
    DrawText(stat.strLevel().c_str(), 490, 590, 60, {255,255,255,255});

    Rectangle rec({grid->getPosition().x-4, grid->getPosition().y-4, (float) grid->getGridWidth()*grid->getSize() + 8, (float)grid->getGridHeight()*grid->getSize() + 8});
    DrawRectangleLinesEx(rec, 4.0,{255,255,255,255});
    //DrawText(grid->strGrid().c_str(), 900, 20, 20, {255,255,255,255});
}

unique_ptr<Block> Game::generateBlock(){
    int position[2] = {0,grid->getGridWidth()/2};//{0,generateRandomNumber(grid_width-3)};//TODO: Talvez tirar essa geração aleatória
    int idx = generateRandomNumber(colors.size()-2) + 1;
    return make_unique<Block>(position, grid->getSize(), idx, colors[idx]);
}

int Game::generateRandomNumber(int limit){
    mt19937 gerador(std::time(0));     
    uniform_int_distribution<int> distribuicao(0, limit);
    return distribuicao(gerador);
}

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
}

void Game::update(){
    if(delayToGoDown<actualSpeed){
        delayToGoDown++;
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
    grid->insertBlock(getBlock());
    swap(block, nextBlock);
    nextBlock = generateBlock();
}

void Game::checkGameOver(){
    if(grid->isThereABlockInTheLine(0)){
        gameOver = true;
    }
}

void Game::getMovement(){
    if(IsKeyDown(KEY_DOWN) && !goDown){
        actualSpeed = fastSpeed;
        goDown = true;
    }
    else if (IsKeyUp(KEY_DOWN) && goDown){
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

void Game::rotateBlock(){
    block->rotate();
}

void Game::moveBlock(char direction){
    block->moveDirection(direction);
}

bool Game::CheckCollisionWall(char direction){
    for(int i=0;i<block->getBlocks().size();i++){
        int posX = block->getPosition()[0] + block->getBlocks()[i].x;
        int posY = block->getPosition()[1] + block->getBlocks()[i].y;
        if(direction=='R' && (posY + 1 >= grid->getGridWidth() || grid->getColorXY(posX, posY+1)!=grid->getBackgroundColor())){
            return true;
        }
        else if (direction=='L' && (posY - 1 < 0 || grid->getColorXY(posX, posY-1)!=grid->getBackgroundColor())){
            return true;
        }
    }
    return false;
}

bool Game::checkCollisionFloor(){
    for(int i=0;i<block->getBlocks().size();i++){
        int posX = block->getPosition()[0] + block->getBlocks()[i].x;
        int posY = block->getPosition()[1] + block->getBlocks()[i].y;
        if(posX+1>=grid->getGridHeight() || grid->getColorXY(posX+1, posY)!=grid->getBackgroundColor()) return true;
    }
    return false;
}

bool Game::checkCollisionFloor(int positionX){
    for(int i=0;i<block->getBlocks().size();i++){
        int posX = positionX + block->getBlocks()[i].x;
        int posY = block->getPosition()[1] + block->getBlocks()[i].y;
        if(posX+1>=grid->getGridHeight() || grid->getColorXY(posX+1, posY)!=grid->getBackgroundColor()) return true;
    }
    return false;
}

int Game::getProjectionLine(){
    for(int i=block->getPosition()[0];i<grid->getGridHeight();i++){
        if(checkCollisionFloor(i)){
            return i;
        }
    }
    return -1;
}

bool Game::getGameOver(){
    return gameOver;
}

Grid Game::getGrid(){
    return *grid;
}

Grid& Game::getGridRef(){
    return *grid;
}

Stat Game::getStat(){
    return stat;
}

Stat& Game::getStatRef(){
    return stat;
}

Block Game::getBlock(){
    return *block;
}

Block& Game::getBlockRef(){
    return *block;
}

Block* Game::getCurrentBlockPtr() {
    return block.get(); // return the pointer
}

Block Game::getNextBlock(){
    return *nextBlock;
}

Block& Game::getNextBlockRef(){
    return *nextBlock;
}

void Game::spawnBlock(int x, int y, int id, int rotationState){
    int position[2] = {x, y};
    int idx = 9; //TODO: Color
    this->block = std::make_unique<Block>(position, id, grid->getSize(), idx, colors[idx]);
    (this->block)->setRotation(rotationState);
}
