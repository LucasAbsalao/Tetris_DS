#pragma once

// Represents a relative position inside a block
struct Position {
    int x;  // Row offset
    int y;  // Column offset

    // Creates a position with given coordinates
    Position(int x, int y) : x(x), y(y) {}
};
