#pragma once
#include<string>
#include<raylib.h>
#include<array>
#include<vector>
#include<iostream>

class Stat{
    private:
        int score;
        int level;
        std::vector<int> how_many_blocks; //TODO: E se eu tiver mais de 7 blocos?????? Por isso troquei para vector
        std::array<int, 4> points;

    public:
        Stat();
        Stat(int qtt_of_blocks);
        void resetStats();
        void updateScore(int lines_destroyed);
        void updateLevel();
        void update(int lines_destroyed);
        void updateHowManyBlocks(int type_of_block);

        int getScore();
        int getLevel();
        std::vector<int> getHowManyBlocks();
        std::string strScore();
        std::string strLevel();

};