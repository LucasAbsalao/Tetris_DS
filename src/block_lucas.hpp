#pragma once

#include<raylib.h>
#include<vector>
#include<array>
#include<ostream>

using namespace std; 

class Block{
    private:
        array<int,2> position;
        vector<Vector2> blocks;
        int size;
        Color color;
        int idx; //TODO

    public:
        Block(int position[2], int size, int idx, Color color);
        void moveDirection(char direction);
        void move(int x, int y); //Colocar como private
        void draw(Vector2 offset);
        void drawProjection(Vector2 offset, int line);
        template<class charT, class charTraits>
        friend basic_ostream<charT,charTraits>& operator <<(basic_ostream<charT, charTraits>& aStream, const Block& block);
        array<int,2> getPosition() const;
        vector<Vector2> getBlocks() const;
        int getColor() const;
        int *GetBoundary() const;
};

template<class charT, class charTraits>
basic_ostream<charT, charTraits>& operator <<(basic_ostream<charT, charTraits>& aStream, const Block& block){
    aStream << block.getPosition()[0] << ", " << block.getPosition()[1] << ": {";
    for(int i=0;i<4;i++){
        aStream << "(" << block.blocks[i].x << ", " << block.blocks[i].y << ") ";
    }
    aStream << "}";
    return aStream;
}