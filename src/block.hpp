#pragma once

#include <raylib.h>
#include <vector>
#include <array>
#include <map>
#include <ostream>

#include "position.hpp"

using namespace std;

// Represents a Tetris block (tetromino)
class Block {
private:
    // Position of the block in the grid (row, column)
    array<int,2> position;

    // All rotation states of the block
    // Each rotation contains 4 relative positions
    map<int, vector<Position>> blocks;

    // Size of each cell in pixels
    int size;

    // Identifier used to choose the block color
    int idx_color;

    // Block type identifier
    int id;

    // Current rotation state index
    int rotationState;

public:
    // Creates a block at a given position with a given size and color ID
    Block(int position[2], int size, int idx_color);

    // Moves the block in a given direction (left, right, down)
    void moveDirection(char direction);

    // Moves the block by a given offset
    void move(int x, int y);

    // Draws the block on the screen using a given color
    void draw(Vector2 offset, Color color) const;

    // Draws the projected landing position of the block
    void drawProjection(Vector2 offset, int line, Color color) const;

    // Rotates the block to the next rotation state
    void rotate();

    // Outputs the block state to a stream (for debugging)
    template<class charT, class charTraits>
    friend basic_ostream<charT,charTraits>& operator<<(basic_ostream<charT,charTraits>& aStream, const Block& block);

    // Returns the current position of the block
    array<int,2> getPosition() const;

    // Returns the cells of the block for the current rotation
    vector<Position> getBlocks() const;

    // Returns the color identifier of the block
    int getColorId() const;

    // Returns the block boundary for collision checks
    int* GetBoundary() const;
};

// Stream operator implementation for printing block information
template<class charT, class charTraits>
basic_ostream<charT, charTraits>& operator<<(basic_ostream<charT, charTraits>& aStream, const Block& block) {
    aStream << block.getPosition()[0] << ", " << block.getPosition()[1] << ": {";
    for (int i = 0; i < 4; i++) {
        aStream << "("
                << block.blocks.at(block.rotationState)[i].x << ", "
                << block.blocks.at(block.rotationState)[i].y << ") ";
    }
    aStream << "}";
    return aStream;
}
