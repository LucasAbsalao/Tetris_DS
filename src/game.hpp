#pragma once

#include<vector>
#include<raylib.h>
#include<iostream>
#include<memory>
#include<random>
#include<ctime>

#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"
#include "colors.hpp"


using namespace std;

class Game
{
    private:
        unique_ptr<Grid> grid;
        vector<Color> colors;
        unique_ptr<Block> block;
        unique_ptr<Block> nextBlock;
        Texture2D backgroundImg;
        Stat stat;

        int position_x;
        int position_y;
        int delayToGoDown;
        int normalSpeed;
        int fastSpeed;
        int actualSpeed;
        bool goDown;
        bool gameOver;

        

    public:
        Game(int normalSpeed, int fast_speed, Texture2D background);
        Game(int normalSpeed, int fastSpeed, Texture2D background, float position_x, float position_y);
        unique_ptr<Block> generateBlock();
        int generateRandomNumber(int limit);
        void draw();
        void drawUI();
        virtual void run();
        void getMovement();
        void update();
        virtual void moveBlock(char direction);
        virtual void rotateBlock();
        virtual void insertBlockInGrid();
        virtual void updateGridStat(int count_lines, int completed_line);
        bool CheckCollisionWall(char direction);
        bool checkCollisionFloor();
        bool checkCollisionFloor(int positionX);
        int getProjectionLine(); 
        void checkGameOver();

        bool getGameOver();
        Block getBlock();
        Block getNextBlock();
        Stat getStat();
        Grid getGrid();

    protected:
        Block& getBlockRef();
        Block& getNextBlockRef();
        Block* getCurrentBlockPtr();
        Grid& getGridRef();
        Stat& getStatRef();
        virtual void setGameOver();
        void spawnBlock(int x, int y, int id, int rotationState);
};