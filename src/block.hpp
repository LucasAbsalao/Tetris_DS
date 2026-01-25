#pragma once

#include <vector>
#include "position.hpp"

// Polymorphic base class for all pieces (tetrominoes / pentominoes)
class Block {
protected:
    int row = 0;               // anchor row in the grid
    int col = 0;               // anchor column in the grid
    int rotationState = 0;     // current rotation index
    int id = 0;                // piece id (also used as colorId)

    // Each rotation is a list of local cell positions (size 4 or 5)
    std::vector<std::vector<Position>> rotations;

public:
    virtual ~Block() = default;

    // Returns local cells of current rotation
    const std::vector<Position>& getCells() const {
        return rotations[rotationState];
    }

    // Rotate to next rotation
    virtual void rotate();

    // Move anchor
    void move(int dRow, int dCol);
    void moveDirection(char direction);

    // Rotation helpers
    int getRotationState() const noexcept { return rotationState; }
    void setRotationState(int s);

    // Getters
    int getId() const noexcept { return id; }
    int getRow() const noexcept { return row; }
    int getCol() const noexcept { return col; }

    // Convenience: returns current rotation cells
    std::vector<Position> getBlocks() const {
        return getCells();
    }
};
