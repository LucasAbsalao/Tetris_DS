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

int Stat::updateScore(int lines_destroyed){
    score += points[lines_destroyed]*(level+1);
    return score;
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