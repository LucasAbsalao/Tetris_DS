#include<vector>
#include<raylib.h>
#include<iostream>
#include<memory>
#include<random>
#include<ctime>

#include"colors.hpp"
#include"block_lucas.hpp"

using namespace std;

class Grid
{
    private:
        vector<vector<int>> grid;
        vector<int> check_how_many_blocks;
        vector<Color> colors;
        int grid_width;
        int grid_height;
        int size;
        int backgroundColor;
        Vector2 position;

    public:
        Grid(int grid_width, int grid_height, int size, Vector2 position);
        Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor);
        string strGrid();
        void draw();
        void setGrid(int x, int y, int idx_color);
        void insertBlock(Block block); //TODO: talvez modificar isso
        int getCompletedLine(int& count);
        void reallocateLines(int line);
        void resetLine(int idx_line);
        //TODO fazer os gets

        int getBackgroundColor();
        Vector2 getPosition();
        int getGridHeight();
        int getGridWidth();
        int getSize();
        int getColorXY(int x, int y);
};