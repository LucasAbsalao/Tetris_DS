#include "game_lucas.hpp"
#include "colors.hpp"

Game::Game(int normalSpeed, int fast_speed, Texture2D background): grid(make_unique<Grid>(10,24,25,(Vector2){125,65},0)), 
                                                                   colors(Colors::getColors()), 
                                                                   block(generateBlock()), 
                                                                   nextBlock(generateBlock()),
                                                                   backgroundImg(background),
                                                                   stat(Stat()),
                                                                   normalSpeed(normalSpeed),
                                                                   fastSpeed(fastSpeed),
                                                                   actualSpeed(normalSpeed),
                                                                   goDown(false),
                                                                   delayToGoDown(0)
{}

void Game::draw(){
    DrawTextureEx(backgroundImg, (Vector2){0.0,0.0}, 0.0f,1.0,WHITE);
    DrawTextureEx(backgroundImg, (Vector2){(float)backgroundImg.width,0.0}, 0.0f,1.0,WHITE);

    char str_score[50];
    DrawText("TETRIS", 145, 10, 50, {255,255,255,255});
    sprintf(str_score, "Score: %d", stat.getScore());
    DrawText(str_score, 400, 50, 30, {255,255,255,255});
    DrawText(grid->strGrid().c_str(), 900, 20, 20, {255,255,255,255});

    grid->draw();
    Rectangle rec({grid->getPosition().x, grid->getPosition().y, (float)grid->getGridWidth()*grid->getSize(), (float)grid->getGridHeight()*grid->getSize()});
    DrawRectangleLinesEx(rec, 4.0,{255,255,255,255});
    if(block!=nullptr){ //DrawBlocks
        block->draw(grid->getPosition());
        int projection_line = getProjectionLine();
        if(projection_line!=-1) block->drawProjection(grid->getPosition(), projection_line);
    }
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
    if(IsKeyDown(KEY_DOWN) && !goDown){ //TODO: handle input
        actualSpeed = fastSpeed;
        goDown = true;
    }
    else if (IsKeyUp(KEY_DOWN) && goDown){
        actualSpeed = normalSpeed;
        delayToGoDown = 0;
        goDown = false;
    }

    getMovement();
    update();

    int count_lines = 0;
    int completed_line = grid->getCompletedLine(count_lines);
    stat.updateScore(count_lines);
    if(completed_line!=-1) grid->reallocateLines(completed_line);
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


void Game::getMovement(){
    if(IsKeyPressed(KEY_RIGHT) && !CheckCollisionWall('R')){
        block->moveDirection('R');
    }
    else if(IsKeyPressed(KEY_LEFT) && !CheckCollisionWall('L')){
        block->moveDirection('L');
    }
}

void Game::update(){
    if(delayToGoDown<actualSpeed){
        delayToGoDown++;
    }
    else{
        if(checkCollisionFloor()){
            grid->insertBlock(getBlock());
            swap(block, nextBlock);
            nextBlock = generateBlock();
        }
        else{
            block->moveDirection('D');
        }
        delayToGoDown=0;
    }
}


Block Game::getBlock(){
    return *block;
}