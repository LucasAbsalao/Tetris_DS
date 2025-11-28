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
        int grid_width;
        int grid_height;
        int size;
        int backgroundColor;
        vector<Color> colors;
        Vector2 position = {20,20};
        unique_ptr<Block> b;

    public:
        Grid(int grid_width, int grid_height, int size);
        Grid(int grid_width, int grid_height, int size, int backgroundColor);
        void generateBlock();
        int generateRandomNumberWidth();
        void draw();
        void update(int& count, int limit);
        Block getBlock();
        static vector<Color> getColors();
};