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
#include "tetrominoes.hpp"

// Core game class: stores game state and applies Tetris rules
class Game {
    private:
        std::unique_ptr<Grid> grid;        // Game board
        std::unique_ptr<Block> block;      // Current falling piece
        std::unique_ptr<Block> nextBlock;  // Next piece
        Stat stat;                         // Score / level

        int position_x;
        int position_y;
        int delayToGoDown;
        int normalSpeed;
        int fastSpeed;
        int actualSpeed;
        bool goDown;
        bool gameOver;

        std::mt19937 rng;                  // RNG (seeded once)
        int lastId;                        // Avoid same piece twice in a row

        int randomInt(int min, int max);
        bool collidesAt(const Block& b, int anchorRow, int anchorCol) const;

        

    public:
        Game(int normalSpeed, int fastSpeed);
        Game(int normalSpeed, int fastSpeed, float position_x, float position_y);
        std::unique_ptr<Block> generateBlock();
        std::unique_ptr<Block> generateSpecificBlock(int id);
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
        bool checkCollisionFloor(int positionX) const;
        void checkGameOver();

        bool getGameOver();
        int getProjectionLine() const; 
        const Grid& getGrid() const;
        const Block& getCurrentBlock() const;
        const Block& getNextBlock() const;
        const Stat& getStats() const;

    protected:
        Block& getBlockRef();
        Block& getNextBlockRef();
        Block* getCurrentBlockPtr();
        Grid& getGridRef();
        Stat& getStatRef();
        virtual void setGameOver();
        void spawnBlock(int x, int y, int id, int rotationState);
};
