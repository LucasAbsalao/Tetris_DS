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
