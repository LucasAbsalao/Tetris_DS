#include<raylib.h>
#include<iostream>
#include"block.hpp"

Block::Block(int position[2], int size, int idx_color, Color color): idx_color(idx_color), position({position[0], position[1]}), color(color), size(size), rotationState(0){
    id = 1;
    blocks[0] = { {0,0}, {0,1}, {1,1}, {1,2} };
    blocks[1] = { {0,2}, {1,1}, {1,2}, {2,1} };
    blocks[2] = { {1,0}, {1,1}, {2,1}, {2,2} };
    blocks[3] = { {0,1}, {1,0}, {1,1}, {2,0} };
}

void Block::rotate(){
    rotationState++;
    if(rotationState==(int)cells.size()){
        rotationState = 0;
    }
}

void Block::moveDirection(char direction){
    switch (direction){
        case 'U':
            position[0]--;
            break;
        case 'D':
            position[0]++;
            break;
        case 'L':
            position[1]--;
            break;
        case 'R':
            position[1]++;
            break;
    }
}

void Block::move(int x, int y){
    position[0] += x;
    position[1] += y;
}

void Block::draw(Vector2 offset){
    for(int i=0;i<blocks[rotationState].size();i++){
        int posY = blocks[rotationState][i].x + position[0];
        int posX = blocks[rotationState][i].y + position[1];
        DrawRectangle(offset.x + size*posX, offset.y + size*posY, size, size, color);
        DrawRectangleLines(offset.x + size*posX, offset.y + size*posY, size, size, {0,0,0,255});
    }
}

array<int,2> Block::getPosition() const{
    return position;
}

vector<Position> Block::getBlocks() const{
    return blocks[rotationState];
}

int Block::getColor() const{
    return idx;
}

void Block::drawProjection(Vector2 offset, int line){
    for(int i=0;i<blocks[rotationState].size();i++){
        int posY = blocks[rotationState][i].x + line;
        int posX = blocks[rotationState][i].y + position[1];
        DrawRectangleLines(offset.x + size*posX, offset.y + size*posY, size, size, color);
    }
}

int *Block::GetBoundary() const{
    int *bounds = (int *) malloc(3*sizeof(int));
    int max = -1;
    for(int i=0;i<blocks[rotationState].size();i++){
        if(blocks[rotationState][i].x>max) max=blocks[rotationState][i].x;
    }
    bounds[0] = max;

    max = 320000;
    for(int i=0;i<blocks[rotationState].size();i++){
        if(blocks[rotationState][i].x<max) max=blocks[rotationState][i].x;
    }
    bounds[1] = max;

    max = -1;
    for(int i=0;i<blocks[rotationState].size();i++){
        if(blocks[rotationState][i].y>max) max=blocks[rotationState][i].x;
    }
    bounds[2] = max;

    return bounds;
}