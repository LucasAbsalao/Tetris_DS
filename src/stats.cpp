#include "stats.hpp"

Stat::Stat(): score(0), level(0), how_many_blocks(7,0), points{40,100,300,1200}
{}

Stat::Stat(int qtt_of_blocks): score(0), level(0), how_many_blocks(qtt_of_blocks, 0), points{40,100,300,1200}
{}


void Stat::resetStats(){
    score = 0;
    level = 0;
    std::fill(how_many_blocks.begin(), how_many_blocks.end(), 0);
}

void Stat::updateScore(int lines_destroyed){
    score += points[lines_destroyed-1]*(level+1);
}

void Stat::updateLevel(){
    if(score>=3000){
        level = 3;
    }
    else if(score>=1500){
        level = 2;
    }
    else if(score>=400){
        level = 1;
    }
}

void Stat::update(int lines_destroyed){
    updateScore(lines_destroyed);
    updateLevel();
}

void Stat::updateHowManyBlocks(int type_of_block){
    how_many_blocks[type_of_block] += 1;
}

int Stat::getScore(){
    return score;
}
int Stat::getLevel(){
    return level;
}
std::vector<int> Stat::getHowManyBlocks(){
  return how_many_blocks;  
}

std::string Stat::strScore(){
    return std::to_string(score);
}

std::string Stat::strLevel(){
    return std::to_string(level);
}