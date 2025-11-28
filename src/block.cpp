#include<raylib.h>
#include"block.hpp"

Block::Block(int position[2], int size, Color color): position({position[0], position[1]}), color(color), size(size){
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
        DrawRectangle(offset.x + size*(posX+1), offset.y + size*(posY+1), size, size, color);
        DrawRectangleLines(offset.x + size*(posX+1), offset.y + size*(posY+1), size, size, {0,0,0,255});
    }
}

array<int,2> Block::getPosition() const{
    return position;
}

vector<Vector2> Block::getBlocks() const{
    return blocks;
}