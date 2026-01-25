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

class Game
{
    private:
        std::unique_ptr<Grid> grid;
        std::vector<Color> colors;
        std::unique_ptr<Block> block;
        std::unique_ptr<Block> nextBlock;
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
        std::unique_ptr<Block> generateBlock();
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
        virtual void receiveAttack(int lines); 
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