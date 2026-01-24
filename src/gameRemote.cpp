#include "gameRemote.hpp"

GameRemote::GameRemote(int normalSpeed, 
                       int fastSpeed, 
                       Texture2D background,
                       int position_x,
                       int position_y,
                       uint8_t id_client): Game(normalSpeed, fastSpeed, background, position_x, position_y),
                                           id_client(id_client)

{} 

void GameRemote::run(){}

void GameRemote::insertBlockInGrid(){}

void GameRemote::rotateBlock(){}

void GameRemote::updateGridStat(int count_lines, int completed_line){}

void GameRemote::moveBlock(char direction){}

void GameRemote::setRemoteBlock(BlockPacket packet){
    Block *block_ptr = getCurrentBlockPtr();
    if(block_ptr!=nullptr && packet.id_block == block_ptr->getID()){
        block_ptr->setPosition(packet.x, packet.y);
        block_ptr->setRotation(packet.rotation);
    }
    else{
        spawnBlock(packet.x, packet.y, packet.id_block, packet.rotation);
    }
}

void GameRemote::setRemoteGrid(GridPacket packet){
    Grid& g = getGridRef();
    for(int i=0;i<packet.lines;i++){
        for(int j=0;j<packet.columns;j++){
            g.setGrid(i, j, packet.grid[i][j]);
        }
    }
}

void GameRemote::setRemoteStat(StatPacket packet){
    Stat& s = getStatRef();
    s.setScore(packet.score);
    s.setLevel(packet.level);
}

void GameRemote::setUsername(std::string name){
    this->username = name;
}

uint8_t GameRemote::getId(){
    return id_client;
}