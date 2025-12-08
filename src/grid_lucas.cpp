#include "grid_lucas.hpp"

Grid::Grid(int grid_width, int grid_height, int size, Vector2 position): grid_width(grid_width), 
                                                       grid_height(grid_height), 
                                                       size(size), 
                                                       colors(Colors::getColors()),
                                                       position(position),
                                                       backgroundColor(0),
                                                       grid(grid_height, vector<int>(grid_width,0)),
                                                       check_how_many_blocks(grid_height,0)
{}

Grid::Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor): grid_width(grid_width), 
                                                       grid_height(grid_height), 
                                                       size(size), 
                                                       backgroundColor(backgroundColor),
                                                       colors(Colors::getColors()),
                                                       position(position),
                                                       grid(grid_height, vector<int>(grid_width,backgroundColor)),
                                                       check_how_many_blocks(grid_height,0)
{}

void Grid::draw(){
    for(int i=0;i<grid_height;i++){
        for(int j=0;j<grid_width;j++){
            DrawRectangle(position.x + size*j, position.y + size*i, size, size, colors[grid[i][j]]);
            DrawRectangleLines(position.x + size*j, position.y + size*i, size, size, colors[1]);
        }
    }
}

void Grid::insertBlock(Block block){
    for(int i = 0; i < block.getBlocks().size(); i++){
        setGrid(block.getPosition()[0] + block.getBlocks()[i].x,
                block.getPosition()[1] + block.getBlocks()[i].y,
                block.getColor());
    }     
}

void Grid::setGrid(int x, int y, int idx_color){
    if(x >= 0 && x < grid_height && y >= 0 && y < grid_width){
        grid[x][y] = idx_color;
        if(idx_color!=backgroundColor)check_how_many_blocks[x]++;
    }
}

int Grid::getColorXY(int x, int y){
    if(x >= 0 && x < grid_height && y >= 0 && y < grid_width){
        return grid[x][y];
    }
    return -1;
}

int Grid::getCompletedLine(int& count){
    int last_line = -1;
    for(int i=0;i<grid_height;i++){
        if(check_how_many_blocks[i]==grid_width){
            count+=1;
            last_line = i;
            resetLine(i);
        }
    }
    return last_line;
}

void Grid::reallocateLines(int line){
    int temp_line = line;
    for(int i=line-1;i>=0;i--){
        if(check_how_many_blocks[i]!=0){
            for(int j=0;j<grid_width;j++){
                setGrid(temp_line,j,grid[i][j]);
            }
            resetLine(i);
            temp_line--;
        }
    }
}

void Grid::resetLine(int idx_line){
    for(int j=0;j<grid_width;j++){
        setGrid(idx_line, j, backgroundColor);
    }
    check_how_many_blocks[idx_line] = 0;
}

string Grid::strGrid(){
    string str_grid;
    for (int i=0;i<grid_height;i++){
        str_grid += to_string(check_how_many_blocks[i]) + ": ";
        for(int j=0;j<grid_width;j++){
            str_grid += to_string(grid[i][j]) + " ";
        }
        str_grid += '\n';
    }
    return str_grid;
}

int Grid::getBackgroundColor(){
    return backgroundColor;
}

Vector2 Grid::getPosition(){
    return position;
}

int Grid::getGridHeight(){
    return grid_height;
}

int Grid::getGridWidth(){
    return grid_width;
}

int Grid::getSize(){
    return size;
}