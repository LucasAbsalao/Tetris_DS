#include<vector>
#include<raylib.h>
#include<iostream>
#include<memory>
#include<random>
#include<ctime>

#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"


using namespace std;

class Game
{
    private:
        vector<Color> colors;
        unique_ptr<Grid> grid;
        unique_ptr<Block> block;
        unique_ptr<Block> nextBlock;
        Texture2D backgroundImg;
        Stat stat;

        int delayToGoDown;
        int normalSpeed;
        int fastSpeed;
        int actualSpeed;
        bool goDown;
        bool gameOver;

    public:
        Game(int normalSpeed, int fast_speed, Texture2D background);
        unique_ptr<Block> generateBlock();
        int generateRandomNumber(int limit);
        void draw();
        void drawUI();
        void run();
        void update();
        void rotateBlock();
        Block getBlock();
        void getMovement();
        bool CheckCollisionWall(char direction);
        bool checkCollisionFloor();
        bool checkCollisionFloor(int positionX);
        int getProjectionLine(); 
        void checkGameOver();

        
        bool getGameOver();
};