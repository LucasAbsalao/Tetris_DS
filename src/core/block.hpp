#pragma once

#include <vector>
#include<ostream>
#include "../util/position.hpp"
#include"../network/networkStructures.hpp"

// Polymorphic base class for all pieces (tetrominoes / pentominoes)
class Block {
    protected:
        int row = 0;               // anchor row in the grid
        int col = 0;               // anchor column in the grid
        int rotationState = 0;     // current rotation index
        int id = 0;                // piece id (also used as colorId)

        // Each rotation is a list of local cell positions (size 4 or 5)
        std::vector<std::vector<Position>> rotations;

    public:
        virtual ~Block() = default;

        // Returns local cells of current rotation
        const std::vector<Position>& getCells() const {
            return rotations[rotationState];
        }

        // Rotate to next rotation
        virtual void rotate();

        // Move anchor
        void move(int dRow, int dCol);
        void moveDirection(char direction);

        // Rotation helpers
        int getRotationState() const noexcept { return rotationState; }
        void setRotationState(int s);
        void setPosition(int x, int y);

        // Getters
        int getId() const noexcept { return id; }
        int getRow() const noexcept { return row; }
        int getCol() const noexcept { return col; }

        // Convenience: returns current rotation cells
        std::vector<Position> getBlocks() const {
            return getCells();
        }

        //Packet to Network
        BlockPacket toPacket(int id) const;

        //Fluc Operator
        template<class charT, class charTraits>
        friend std::basic_ostream<charT,charTraits>& operator <<(std::basic_ostream<charT, charTraits>& aStream, const Block& block);
};

template<class charT, class charTraits>
std::basic_ostream<charT, charTraits>& operator <<(std::basic_ostream<charT, charTraits>& aStream, const Block& block){
    aStream << block.getRow() << ", " << block.getCol() << ": {";
    for (const auto& p : block.getBlocks()){
        aStream << "(" << p.x << ", " << p.y << ") ";
    }
}