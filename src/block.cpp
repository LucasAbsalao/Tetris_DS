#include <raylib.h>
#include <iostream>
#include "block.hpp"

// Builds the block with position, cell size, and a color ID
Block::Block(int position[2], int size, int idx_color)
    : position({position[0], position[1]}),
      blocks(),
      size(size),
      idx_color(idx_color),
      id(1),
      rotationState(0)
{
    // Define the 4 rotation states (each with 4 cells)
    blocks[0] = { {0,0}, {0,1}, {1,1}, {1,2} };
    blocks[1] = { {0,2}, {1,1}, {1,2}, {2,1} };
    blocks[2] = { {1,0}, {1,1}, {2,1}, {2,2} };
    blocks[3] = { {0,1}, {1,0}, {1,1}, {2,0} };
}

// Cycles to the next rotation state
void Block::rotate() {
    rotationState++;
    if (rotationState == (int)blocks.size()) {
        rotationState = 0;
    }
}

// Moves the block one cell in a direction
void Block::moveDirection(char direction) {
    switch (direction) {
        case 'U': position[0]--; break;
        case 'D': position[0]++; break;
        case 'L': position[1]--; break;
        case 'R': position[1]++; break;
    }
}

// Moves the block by a custom offset
void Block::move(int x, int y) {
    position[0] += x;
    position[1] += y;
}

// Draws the block using the provided color
void Block::draw(Vector2 offset, Color color) const {
    for (int i = 0; i < (int)blocks.at(rotationState).size(); i++) {
        int posY = blocks.at(rotationState)[i].x + position[0];
        int posX = blocks.at(rotationState)[i].y + position[1];

        DrawRectangle(offset.x + size * posX, offset.y + size * posY, size, size, color);
        DrawRectangleLines(offset.x + size * posX, offset.y + size * posY, size, size, (Color){0, 0, 0, 255});
    }
}

// Returns the current grid position (row, col)
array<int,2> Block::getPosition() const {
    return position;
}

// Returns the 4 cells for the current rotation
vector<Position> Block::getBlocks() const {
    return blocks.at(rotationState);
}

// Returns the color ID of this block
int Block::getColorId() const {
    return idx_color;
}

// Draws the "ghost" landing position outline using the provided color
void Block::drawProjection(Vector2 offset, int line, Color color) const {
    for (int i = 0; i < (int)blocks.at(rotationState).size(); i++) {
        int posY = blocks.at(rotationState)[i].x + line;
        int posX = blocks.at(rotationState)[i].y + position[1];

        DrawRectangleLines(offset.x + size * posX, offset.y + size * posY, size, size, color);
    }
}

// Computes simple boundaries of the current rotation (allocated array of 3 ints)
int* Block::GetBoundary() const {
    int *bounds = (int*)malloc(3 * sizeof(int));

    // Max x among cells (downwards extent)
    int maxX = -1;
    for (int i = 0; i < (int)blocks.at(rotationState).size(); i++) {
        if (blocks.at(rotationState)[i].x > maxX) maxX = blocks.at(rotationState)[i].x;
    }
    bounds[0] = maxX;

    // Min x among cells (upwards extent)
    int minX = 320000;
    for (int i = 0; i < (int)blocks.at(rotationState).size(); i++) {
        if (blocks.at(rotationState)[i].x < minX) minX = blocks.at(rotationState)[i].x;
    }
    bounds[1] = minX;

    // Max y among cells (rightwards extent)
    int maxY = -1;
    for (int i = 0; i < (int)blocks.at(rotationState).size(); i++) {
        if (blocks.at(rotationState)[i].y > maxY) maxY = blocks.at(rotationState)[i].y; // fixed: was using x
    }
    bounds[2] = maxY;

    return bounds;
}
