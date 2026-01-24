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

Block::Block(int position[2], int id, int size, int idx_color, Color color): idx_color(idx_color), position({position[0], position[1]}), color(color), id(id), size(size), rotationState(0){
    blocks[0] = { {0,0}, {0,1}, {1,1}, {1,2} };
    blocks[1] = { {0,2}, {1,1}, {1,2}, {2,1} };
    blocks[2] = { {1,0}, {1,1}, {2,1}, {2,2} };
    blocks[3] = { {0,1}, {1,0}, {1,1}, {2,0} };
}

void Block::rotate(){
    rotationState++;
    if(rotationState==(int)blocks.size()){
        rotationState = 0;
    }
}

void Block::setRotation(int rotationState){
    this->rotationState = rotationState;
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

void Block::setPosition(int x, int y){
    position[0] = x;
    position[1] = y;
}

BlockPacket Block::toPacket(int id){
    BlockPacket packet;
    packet.type = MessageType::BLOCK;
    packet.id = id;
    packet.id_block = static_cast<uint8_t>(id);
    packet.rotation = static_cast<uint8_t>(rotationState);
    packet.x = static_cast<uint8_t>(position[0]);
    packet.y = static_cast<uint8_t>(position[1]);
    return packet;
}

void Block::draw(Vector2 offset){
    for(int i=0;i<blocks.at(rotationState).size();i++){
        int posY = blocks.at(rotationState)[i].x + position[0];
        int posX = blocks.at(rotationState)[i].y + position[1];
        DrawRectangle(offset.x + size*posX, offset.y + size*posY, size, size, color);
        DrawRectangleLines(offset.x + size*posX, offset.y + size*posY, size, size, {0,0,0,255});
    }
}

array<int,2> Block::getPosition() const{
    return position;
}

vector<Position> Block::getBlocks() const{
    return blocks.at(rotationState);
}

int Block::getColor() const{
    return idx_color;
}

void Block::drawProjection(Vector2 offset, int line){
    for(int i=0;i<blocks.at(rotationState).size();i++){
        int posY = blocks.at(rotationState)[i].x + line;
        int posX = blocks.at(rotationState)[i].y + position[1];
        DrawRectangleLines(offset.x + size*posX, offset.y + size*posY, size, size, color);
    }
}

int *Block::GetBoundary() const{
    int *bounds = (int *) malloc(3*sizeof(int));
    int max = -1;
    for(int i=0;i<blocks.at(rotationState).size();i++){
        if(blocks.at(rotationState)[i].x>max) max=blocks.at(rotationState)[i].x;
    }
    bounds[0] = max;

    max = 320000;
    for(int i=0;i<blocks.at(rotationState).size();i++){
        if(blocks.at(rotationState)[i].x<max) max=blocks.at(rotationState)[i].x;
    }
    bounds[1] = max;

    max = -1;
    for(int i=0;i<blocks.at(rotationState).size();i++){
        if(blocks.at(rotationState)[i].y>max) max=blocks.at(rotationState)[i].x;
    }
    bounds[2] = max;

    return bounds;
}

int Block::getID() const{
    return this->id;
}