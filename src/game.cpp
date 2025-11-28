#include "game.hpp"

Grid::Grid(int grid_width, int grid_height, int size): grid_width(grid_width), 
                                                       grid_height(grid_height), 
                                                       size(size), 
                                                       colors(getColors()),
                                                       grid(grid_height, vector<int>(grid_width,0))
{}

Grid::Grid(int grid_width, int grid_height, int size, int backgroundColor): grid_width(grid_width), 
                                                       grid_height(grid_height), 
                                                       size(size), 
                                                       backgroundColor(backgroundColor),
                                                       colors(getColors()),
                                                       grid(grid_height, vector<int>(grid_width,backgroundColor))
{}

void Grid::draw(){
    for(int i=0;i<grid_height;i++){
        for(int j=0;j<grid_width;j++){
            DrawRectangle(position.x + size*(j+1), position.y + size*(i+1), size, size, colors[grid[i][j]]);
            DrawRectangleLines(position.x + size*(j+1), position.y + size*(i+1), size, size, colors[0]);
        }
    }
    if(b!=nullptr){
        b->draw(position);
    }
}

void Grid::generateBlock(){
    int position[2] = {0,generateRandomNumberWidth()};
    cout << position[1] << endl;
    b = make_unique<Block>(position, 20, YELLOW);
    cout << *b << endl;
}

int Grid::generateRandomNumberWidth(){
    mt19937 gerador(std::time(0));     
    uniform_int_distribution<int> distribuicao(0, grid_width);
    return distribuicao(gerador);
}

vector<Color> Grid::getColors(){
    Color black = {0,0,0,255};
    vector<Color> c = {
        black, DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };
    return c;
}

Block Grid::getBlock(){
    return *b;
}

void Grid::update(int& count, int limit){
    if(count<limit){
        count++;
    }
    else{
        b->move('D');
        count=0;
    }
}