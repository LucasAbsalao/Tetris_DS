#include "stats.hpp"

// Initializes score, level, block counters, and scoring table
Stat::Stat()
    : score(0),
      level(0),
      how_many_blocks(7, 0),
      points{40, 100, 300, 1200}
{}

// Initializes stats with a custom number of block types
Stat::Stat(int qtt_of_blocks)
    : score(0),
      level(0),
      how_many_blocks(qtt_of_blocks, 0),
      points{40, 100, 300, 1200}
{}

// Resets all statistics to initial values
void Stat::resetStats() {
    score = 0;
    level = 0;
    std::fill(how_many_blocks.begin(), how_many_blocks.end(), 0);
}

// Updates score based on how many lines were destroyed
void Stat::updateScore(int lines_destroyed) {
    score += points[lines_destroyed - 1] * (level + 1);
}

// Updates level according to the current score
void Stat::updateLevel() {
    if (score >= 3000) {
        level = 3;
    } else if (score >= 1500) {
        level = 2;
    } else if (score >= 400) {
        level = 1;
    }
}

// Updates score and level in one step
void Stat::update(int lines_destroyed) {
    updateScore(lines_destroyed);
    updateLevel();
}

// Increments the usage count of a given block type
void Stat::updateHowManyBlocks(int type_of_block) {
    how_many_blocks[type_of_block] += 1;
}

// Returns the current score
int Stat::getScore() const {
    return score;
}

// Returns the current level
int Stat::getLevel() const {
    return level;
}

// Returns the block usage statistics
std::vector<int> Stat::getHowManyBlocks() const {
    return how_many_blocks;
}

// Returns the score as a string (for display)
std::string Stat::strScore() const {
    return std::to_string(score);
}

// Returns the level as a string (for display)
std::string Stat::strLevel() const {
    return std::to_string(level);
}
