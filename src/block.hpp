#pragma once

#include<raylib.h>
#include<vector>
#include<array>
#include<map>
#include<ostream>

#include "position.hpp"
#include"network/networkStructures.hpp"

class Block{
    private:
        std::array<int,2> position;
        std::map<int, std::vector<Position>> blocks;
        int size;
        Color color;
        int idx_color; //TODO
        int id;
        int rotationState;

    public:
        Block(int position[2], int size, int idx_color, Color color);
        Block(int position[2], int id, int size, int idx_color, Color color);
        void moveDirection(char direction);
        void move(int x, int y); //Colocar como private
        void draw(Vector2 offset);
        void drawProjection(Vector2 offset, int line);
        void rotate();
        void setRotation(int rotationState);
        void setPosition(int x, int y);

        std::array<int,2> getPosition() const;
        std::vector<Position> getBlocks() const;
        int getColor() const;
        int getID() const;
        int *GetBoundary() const;


        BlockPacket toPacket(int id);
        
        template<class charT, class charTraits>
        friend std::basic_ostream<charT,charTraits>& operator <<(std::basic_ostream<charT, charTraits>& aStream, const Block& block);
};

template<class charT, class charTraits>
std::basic_ostream<charT, charTraits>& operator <<(std::basic_ostream<charT, charTraits>& aStream, const Block& block){
    aStream << block.getPosition()[0] << ", " << block.getPosition()[1] << ": {";
    for(int i=0;i<4;i++){
        aStream << "(" << block.blocks.at(block.rotationState)[i].x << ", " << block.blocks.at(block.rotationState)[i].y << ") ";
    }
    aStream << "}";
    return aStream;
}