#include "grid.hpp"

// Creates an empty grid filled with background (0)
Grid::Grid(int grid_width, int grid_height, int size, Vector2 position)
    : grid_width(grid_width),
      grid_height(grid_height),
      size(size),
      colors(Colors::getColors()),
      position(position),
      backgroundColor(0),
      grid(grid_height, std::vector<int>(grid_width, 0)),
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
      grid(grid_height, std::vector<int>(grid_width, backgroundColor)),
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

bool Grid::isThereABlockInTheLine(int line){
    for(int i=0;i<grid_width;i++){
        if(grid[line][i]!=0){
            return true;
        }
    } 
    return false;
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
void Grid::setGrid(int x, int y, int idx_color){
    if(x >= 0 && x < grid_height && y >= 0 && y < grid_width){
        if(idx_color!=backgroundColor && grid[x][y]==backgroundColor) check_how_many_blocks[x]++;
        if(idx_color==backgroundColor && grid[x][y]!=backgroundColor) check_how_many_blocks[x]--;
        grid[x][y] = idx_color;
    }//TODO: Exception
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
    bool line_moving = false;
    for(int i=line-1;i>=0;i--){
        if(check_how_many_blocks[i]!=0 || line_moving==true){
            line_moving = true;
            for(int j=0;j<grid_width;j++){
                setGrid(temp_line,j,grid[i][j]);
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

void Grid::receiveAttack(int hole){
    for(int i=0;i<grid_height-1;i++){
        resetLine(i);
        for(int j=0;j<grid_width;j++){
            setGrid(i, j, grid[i+1][j]);
        }
    }

    for(int j = 0;j < grid_width; j++){
        if(j!=hole){
            setGrid(grid_height-1, j, 15);
        }
        else{
            setGrid(grid_height-1, j, backgroundColor);
        }
    }
}

std::string Grid::strGrid() const{ //TODO: operator string
    std::string str_grid;
    for (int i=0;i<grid_height;i++){
        str_grid += std::to_string(check_how_many_blocks[i]) + ": ";
        for(int j=0;j<grid_width;j++){
            str_grid += std::to_string(grid[i][j]) + " ";
        }
        str_grid += '\n';
    }
    return str_grid;
}

GridPacket Grid::toPacket(int id){
    GridPacket packet;
    packet.type = MessageType::GRID;
    packet.id = id;
    packet.lines = static_cast<uint8_t>(grid_height);
    packet.columns = static_cast<uint8_t>(grid_width);

    std::memset(packet.grid, 0, sizeof(packet.grid));

    for(int i=0;i<grid_height;i++){
        for(int j=0;j<grid_width;j++){
            packet.grid[i][j] = static_cast<uint8_t>(grid[i][j]);
        }
    }

    return packet;
}


// Getters
int Grid::getBackgroundColor() const { return backgroundColor; }
Vector2 Grid::getPosition() const { return position; }
int Grid::getGridHeight() const { return grid_height; }
int Grid::getGridWidth() const { return grid_width; }
int Grid::getSize() const { return size; }
