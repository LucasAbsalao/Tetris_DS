#pragma once

#include <string>
#include <array>
#include <vector>

#include"network/networkStructures.hpp"

// Stores and updates game statistics (score, level, usage)
class Stat {
    private:
        // Current player score
        int score;

        // Current game level
        int level;

        // Counts how many times each block type was used
        std::vector<int> how_many_blocks;

        // Score values for different numbers of cleared lines
        std::array<int, 4> points;

    public:
        // Creates a stat object with default values
        Stat();

        // Creates a stat object with a custom number of block types
        Stat(int qtt_of_blocks);

        // Resets score, level, and counters
        void resetStats();

        // Updates score based on destroyed lines
        void updateScore(int lines_destroyed);

        // Updates level based on progress
        void updateLevel();

        // Updates score and level together
        void update(int lines_destroyed);

        // Updates block usage statistics
        void updateHowManyBlocks(int type_of_block);

        // Returns the current score
        int getScore() const;

        // Returns the current level
        int getLevel() const;

        // Returns block usage data
        std::vector<int> getHowManyBlocks() const;

        // Returns score as a formatted string
        std::string strScore() const;

        // Returns level as a formatted string
        std::string strLevel() const;

        // Returns stat as a packet
        StatPacket toPacket(int id) const;

        void setScore(int score);
        
        void setLevel(int level);
};
