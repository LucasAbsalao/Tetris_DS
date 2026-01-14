#pragma once

#include <vector>
#include <string>
#include <raylib.h>

#include "colors.hpp"
#include "block.hpp"

using namespace std;

// Represents the Tetris board (matrix of color IDs)
class Grid
{
private:
    // Each cell stores a color ID (0 = background)
    vector<vector<int>> grid;

    // Helper vector used to detect completed lines
    vector<int> check_how_many_blocks;

    // Color palette used for rendering
    vector<Color> colors;

    // Grid dimensions (in cells)
    int grid_width;
    int grid_height;

    // Cell size (in pixels)
    int size;

    // Background cell ID
    int backgroundColor;

    // Top-left position of the grid on screen
    Vector2 position;

public:
    // Builds the grid with default background color
    Grid(int grid_width, int grid_height, int size, Vector2 position);

    // Builds the grid with a custom background color ID
    Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor);

    // Returns a string representation (debug)
    string strGrid();

    // Draws the grid on screen
    void draw() const;

    // Sets a single cell to a given color ID
    void setGrid(int x, int y, int idx_color);

    // Inserts a block into the grid (locks it in place)
    void insertBlock(const Block& block);

    // Checks for full lines and returns the first completed line index
    int getCompletedLine(int& count);

    // Removes completed lines and shifts above lines down
    void reallocateLines(int line);

    // Clears a specific line
    void resetLine(int idx_line);

    // Getters
    int getBackgroundColor() const;
    Vector2 getPosition() const;
    int getGridHeight() const;
    int getGridWidth() const;
    int getSize() const;

    // Returns the color ID at (x, y)
    int getColorXY(int x, int y) const;
};
