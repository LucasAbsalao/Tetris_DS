#pragma once

#include <vector>
#include <string>
#include <raylib.h>

#include "colors.hpp"
#include "block.hpp"

using namespace std;

// Represents the Tetris board as a matrix of color IDs
class Grid
{
private:
    vector<vector<int>> grid;           // grid[row][col] = colorId
    vector<int> check_how_many_blocks;  // filled cells count per row
    vector<Color> colors;               // color palette

    int grid_width;                     // number of columns
    int grid_height;                    // number of rows
    int size;                           // cell size in pixels
    int backgroundColor;                // colorId for empty cells
    Vector2 position;                   // top-left pixel position on screen

public:
    // Creates a grid filled with backgroundColor = 0
    Grid(int grid_width, int grid_height, int size, Vector2 position);

    // Creates a grid filled with a custom background colorId
    Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor);

    // Returns a debug string showing the grid contents
    string strGrid() const;

    // Draws the grid cells using the color palette
    void draw() const;

    // Sets one cell to a given colorId
    void setGrid(int x, int y, int idx_color);

    // Writes a block cells into the grid (locks the piece)
    void insertBlock(const Block& block);

    // Detects full lines and clears them
    int getCompletedLine(int& count);

    // Shifts lines down after a clear
    void reallocateLines(int line);

    // Clears one row
    void resetLine(int idx_line);

    // Checks if the row has at least one block
    bool isRowOccupied(int row) const;


    // Basic getters
    int getBackgroundColor() const;
    Vector2 getPosition() const;
    int getGridHeight() const;
    int getGridWidth() const;
    int getSize() const;

    // Returns the colorId stored at a given cell
    int getColorXY(int x, int y) const;
};