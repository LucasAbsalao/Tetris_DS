#include<vector>
#include<raylib.h>
#include<iostream>
#include<memory>
#include<random>
#include<ctime>

#include "block.hpp"


using namespace std;

class Grid
{
    private:
        vector<vector<int>> grid;
        vector<int> check_how_many_blocks;
        int grid_width;
        int grid_height;
        int size;
        int backgroundColor;
        vector<Color> colors;
        Vector2 position;
        unique_ptr<Block> b;

    public:
        Grid(int grid_width, int grid_height, int size, Vector2 position);
        Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor);
        void generateBlock(); //Game
        int generateRandomNumber(int limit); //Game
        void draw();
        void setGrid(int x, int y, int idx_color);
        void update(int& count, int limit); //Game
        Block getBlock();//Game
        void getMovement();//Game
        bool CheckCollisionWall(char direction); //Game
        bool checkCollisionFloor(); //Game
        int getCompletedLine(int& score);
        void reallocateLines(); //TODO
        void resetLine(int idx_line);
        static vector<Color> getColors();//Game
};