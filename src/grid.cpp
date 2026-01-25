#include "grid.hpp"

// Creates an empty grid filled with background (0)
Grid::Grid(int grid_width, int grid_height, int size, Vector2 position)
    : grid_width(grid_width),
      grid_height(grid_height),
      size(size),
      colors(Colors::getColors()),
      position(position),
      backgroundColor(0),
      grid(grid_height, vector<int>(grid_width, 0)),
      check_how_many_blocks(grid_height, 0)
{}

// Creates an empty grid filled with a custom background color ID
Grid::Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor)
    : grid_width(grid_width),
      grid_height(grid_height),
      size(size),
      backgroundColor(backgroundColor),
      colors(Colors::getColors()),
      position(position),
      grid(grid_height, vector<int>(grid_width, backgroundColor)),
      check_how_many_blocks(grid_height, 0)
{}

// Draws each cell using its color ID
void Grid::draw() const {
    for (int i = 0; i < grid_height; i++) {
        for (int j = 0; j < grid_width; j++) {
            DrawRectangle(position.x + size * j, position.y + size * i, size, size, colors[grid[i][j]]);
            DrawRectangleLines(position.x + size * j, position.y + size * i, size, size, colors[1]);
        }
    }
}

// Locks the current block into the grid matrix
void Grid::insertBlock(const Block& block) {
    auto cells = block.getBlocks(); // 4 cells of the current rotation

    for (const auto& p : cells) {
        int x = block.getRow() + p.x;
        int y = block.getCol() + p.y;
        setGrid(x, y, block.getId()); // id is used as colorId
    }
}

// Sets one grid cell to a given color ID
void Grid::setGrid(int x, int y, int idx_color) {
    if (x >= 0 && x < grid_height && y >= 0 && y < grid_width) {
        grid[x][y] = idx_color;

        // Tracks how many filled cells exist in each row
        if (idx_color != backgroundColor) check_how_many_blocks[x]++;
    }
}

// Returns the color ID stored at (x, y)
int Grid::getColorXY(int x, int y) const {
    if (x >= 0 && x < grid_height && y >= 0 && y < grid_width) {
        return grid[x][y];
    }
    return -1;
}

// Finds completed lines and clears them
int Grid::getCompletedLine(int& count) {
    int last_line = -1;

    for (int i = 0; i < grid_height; i++) {
        if (check_how_many_blocks[i] == grid_width) {
            count++;
            last_line = i;
            resetLine(i);
        }
    }
    return last_line;
}

// Shifts lines down after a line clear
void Grid::reallocateLines(int line) {
    int temp_line = line;

    for (int i = line - 1; i >= 0; i--) {
        if (check_how_many_blocks[i] != 0) {
            for (int j = 0; j < grid_width; j++) {
                setGrid(temp_line, j, grid[i][j]);
            }
            resetLine(i);
            temp_line--;
        }
    }
}

// Clears one row
void Grid::resetLine(int idx_line) {
    for (int j = 0; j < grid_width; j++) {
        setGrid(idx_line, j, backgroundColor);
    }
    check_how_many_blocks[idx_line] = 0;
}

// Builds a debug string for printing the grid
string Grid::strGrid() const {
    string str_grid;

    for (int i = 0; i < grid_height; i++) {
        str_grid += to_string(check_how_many_blocks[i]) + ": ";
        for (int j = 0; j < grid_width; j++) {
            str_grid += to_string(grid[i][j]) + " ";
        }
        str_grid += '\n';
    }
    return str_grid;
}

// True if at least one cell in the row is not background
bool Grid::isRowOccupied(int row) const {
    for (int c = 0; c < grid_width; c++) {
        if (grid[row][c] != backgroundColor) return true;
    }
    return false;
}

// Getters
int Grid::getBackgroundColor() const { return backgroundColor; }
Vector2 Grid::getPosition() const { return position; }
int Grid::getGridHeight() const { return grid_height; }
int Grid::getGridWidth() const { return grid_width; }
int Grid::getSize() const { return size; }