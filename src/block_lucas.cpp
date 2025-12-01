#include<raylib.h>
#include<iostream>
#include"block_lucas.hpp"

Block::Block(int position[2], int size, int idx, Color color): idx(idx), position({position[0], position[1]}), color(color), size(size){
    blocks.push_back({0,0});
    blocks.push_back({0,1});
    blocks.push_back({1,0});
    blocks.push_back({1,1});
}

void Block::move(char direction){
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

void Block::draw(Vector2 offset){
    for(int i=0;i<blocks.size();i++){
        int posY = blocks[i].x + position[0];
        int posX = blocks[i].y + position[1];
        DrawRectangle(offset.x + size*posX, offset.y + size*posY, size, size, color);
        DrawRectangleLines(offset.x + size*posX, offset.y + size*posY, size, size, {0,0,0,255});
    }
}

array<int,2> Block::getPosition() const{
    return position;
}

vector<Vector2> Block::getBlocks() const{
    return blocks;
}

int Block::getColor() const{
    return idx;
}

int * Block::GetBoundary() const{
    int *bounds = (int *) malloc(3*sizeof(int));
    int max = -1;
    for(int i=0;i<blocks.size();i++){
        if(blocks[i].x>max) max=blocks[i].x;
    }
    bounds[0] = max;

    max = 320000;
    for(int i=0;i<blocks.size();i++){
        if(blocks[i].x<max) max=blocks[i].x;
    }
    bounds[1] = max;

    max = -1;
    for(int i=0;i<blocks.size();i++){
        if(blocks[i].y>max) max=blocks[i].x;
    }
    bounds[2] = max;

    return bounds;
}