#include "game_lucas.hpp"

Grid::Grid(int grid_width, int grid_height, int size, Vector2 position): grid_width(grid_width), 
                                                       grid_height(grid_height), 
                                                       size(size), 
                                                       colors(getColors()),
                                                       position(position),
                                                       backgroundColor(0),
                                                       grid(grid_height, vector<int>(grid_width,0)),
                                                       check_how_many_blocks(grid_height,0)
{}

Grid::Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor): grid_width(grid_width), 
                                                       grid_height(grid_height), 
                                                       size(size), 
                                                       backgroundColor(backgroundColor),
                                                       colors(getColors()),
                                                       position(position),
                                                       grid(grid_height, vector<int>(grid_width,backgroundColor)),
                                                       check_how_many_blocks(grid_height,0)
{}

void Grid::draw(){
    Rectangle rec({position.x, position.y, (float)grid_width*size, (float)grid_height*size});
    for(int i=0;i<grid_height;i++){
        for(int j=0;j<grid_width;j++){
            DrawRectangle(position.x + size*j, position.y + size*i, size, size, colors[grid[i][j]]);
            DrawRectangleLines(position.x + size*j, position.y + size*i, size, size, colors[5]);
        }
    }
    if(b!=nullptr){
        b->draw(position);
    }
    DrawRectangleLinesEx(rec, 4.0,{255,255,255,255});
}

void Grid::generateBlock(){
    int position[2] = {0,grid_width/2};//{0,generateRandomNumber(grid_width-3)};//TODO: Talvez tirar essa geração aleatória
    cout << position[1] << endl;
    int idx = generateRandomNumber(colors.size()-1);
    b = make_unique<Block>(position, size, idx+1, colors[idx+1]);
    cout << *b << endl;
}

int Grid::generateRandomNumber(int limit){
    mt19937 gerador(std::time(0));     
    uniform_int_distribution<int> distribuicao(0, limit);
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

bool Grid::CheckCollisionWall(char direction){
    for(int i=0;i<b->getBlocks().size();i++){
        int posX = b->getPosition()[0] + b->getBlocks()[i].x;
        int posY = b->getPosition()[1] + b->getBlocks()[i].y;
        if(direction=='R' && (posY + 1 >= grid_width || grid[posX][posY+1]!=backgroundColor)){
            return true;
        }
        else if (direction=='L' && (posY - 1 < 0 || grid[posX][posY-1]!=backgroundColor)){
            return true;
        }
    }
    return false;
}

bool Grid::checkCollisionFloor(){
    for(int i=0;i<b->getBlocks().size();i++){
        int posX = b->getPosition()[0] + b->getBlocks()[i].x;
        int posY = b->getPosition()[1] + b->getBlocks()[i].y;
        if(posX+1>=grid_height || grid[posX+1][posY]!=backgroundColor) return true;
    }
    return false;
}

void Grid::getMovement(){
    if(IsKeyPressed(KEY_RIGHT) && !CheckCollisionWall('R')){
        b->move('R');
    }
    else if(IsKeyPressed(KEY_LEFT) && !CheckCollisionWall('L')){
        b->move('L');
    }
}

void Grid::update(int& count, int limit){
    if(count<limit){
        count++;
    }
    else{
        if(checkCollisionFloor()){
            for(int i = 0; i < b->getBlocks().size(); i++){
                setGrid(b->getPosition()[0] + b->getBlocks()[i].x,
                        b->getPosition()[1] + b->getBlocks()[i].y,
                        b->getColor());
            }
            generateBlock();
        }
        else{
            b->move('D');
        }
        count=0;
    }
}

void Grid::setGrid(int x, int y, int idx_color){
    if(x >= 0 && x < grid_height && y >= 0 && y < grid_width){
        grid[x][y] = idx_color;
        if(idx_color!=backgroundColor)check_how_many_blocks[x]++;
    }
}

int Grid::getCompletedLine(int& score){
    int last_line = -1;
    for(int i=0;i<grid_height;i++){
        if(check_how_many_blocks[i]==grid_width){
            score+=100;
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
    for(int i=0;i<check_how_many_blocks.size();i++) cout << check_how_many_blocks[i]<<" ";
    cout<<endl;
    string str_grid;
    for (int i=0;i<grid_height;i++){
        str_grid += check_how_many_blocks[i] + ": ";
        for(int j=0;j<grid_width;j++){
            str_grid += to_string(grid[i][j]) + " ";
        }
        str_grid += '\n';
    }
    return str_grid;
}