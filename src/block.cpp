#include "block.hpp"

// Rotate to next rotation (supports pieces with 1, 2 or 4 rotations)
void Block::rotate() {
    if (rotations.empty()) return;
    rotationState = (rotationState + 1) % (int)rotations.size();
}

// Force rotation state safely
void Block::setRotationState(int s) {
    if (rotations.empty()) {
        rotationState = 0;
        return;
    }
    int n = (int)rotations.size();
    rotationState = ((s % n) + n) % n;
}

// Move anchor position
void Block::move(int dRow, int dCol) {
    row += dRow;
    col += dCol;
}

// Move using direction
void Block::moveDirection(char direction) {
    switch (direction) {
        case 'U': row--; break;
        case 'D': row++; break;
        case 'L': col--; break;
        case 'R': col++; break;
    }
}

void Block::setPosition(int x, int y){
    row = x;
    col = y;
}

BlockPacket Block::toPacket(int id){
    BlockPacket packet;
    packet.type = MessageType::BLOCK;
    packet.id = id;
    packet.id_block = static_cast<uint8_t>(id);
    packet.rotation = static_cast<uint8_t>(rotationState);
    packet.x = static_cast<uint8_t>(row);
    packet.y = static_cast<uint8_t>(col);
    return packet;
}

// void Block::drawProjection(Vector2 offset, int line){
//     for(int i=0;i<blocks.at(rotationState).size();i++){
//         int posY = blocks.at(rotationState)[i].x + line;
//         int posX = blocks.at(rotationState)[i].y + position[1];
//         DrawRectangleLines(offset.x + size*posX, offset.y + size*posY, size, size, color);
//     }
// }

