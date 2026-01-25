#pragma once

#include<vector>
#include<raylib.h>
#include<iostream>
#include<memory>
#include<random>
#include<ctime>
#include<cstring>

#include"network/networkStructures.hpp"
#include"colors.hpp"
#include"block.hpp"

class Grid
{
    private:
        std::vector<std::vector<int>> grid;
        std::vector<int> check_how_many_blocks;
        std::vector<Color> colors;
        int grid_width;
        int grid_height;
        int size;
        int backgroundColor;
        Vector2 position;

    public:
        Grid(int grid_width, int grid_height, int size, Vector2 position);
        Grid(int grid_width, int grid_height, int size, Vector2 position, int backgroundColor);
        std::string strGrid();
        void draw();
        void setGrid(int x, int y, int idx_color);
        void insertBlock(Block block); //TODO: talvez modificar isso
        int getCompletedLine(int& count);
        void reallocateLines(int line);
        void resetLine(int idx_line);
        bool isThereABlockInTheLine(int line);

        GridPacket toPacket(int id);

        //Receiva an attack from other game;
        void receiveAttack(int hole);

        int getBackgroundColor();
        Vector2 getPosition();
        int getGridHeight();
        int getGridWidth();
        int getSize();
        int getColorXY(int x, int y);
};