#pragma once

#include<raylib.h>
#include<vector>
#include<array>
#include<map>
#include<ostream>

#include "position.hpp"

using namespace std; 

class Block{
    private:
        array<int,2> position;
        map<int, vector<Position>> blocks;
        int size;
        Color color;
        int idx_color; //TODO
        int id;
        int rotationState;

    public:
        Block(int position[2], int size, int idx_color, Color color);
        void moveDirection(char direction);
        void move(int x, int y); //Colocar como private
        void draw(Vector2 offset);
        void drawProjection(Vector2 offset, int line);
        void rotate();
        template<class charT, class charTraits>
        friend basic_ostream<charT,charTraits>& operator <<(basic_ostream<charT, charTraits>& aStream, const Block& block);
        array<int,2> getPosition() const;
        vector<Position> getBlocks() const;
        int getColor() const;
        int *GetBoundary() const;
};

template<class charT, class charTraits>
basic_ostream<charT, charTraits>& operator <<(basic_ostream<charT, charTraits>& aStream, const Block& block){
    aStream << block.getPosition()[0] << ", " << block.getPosition()[1] << ": {";
    for(int i=0;i<4;i++){
        aStream << "(" << block.blocks.at(block.rotationState)[i].x << ", " << block.blocks.at(block.rotationState)[i].y << ") ";
    }
    aStream << "}";
    return aStream;
}