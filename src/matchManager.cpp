#include "matchManager.hpp"

MatchManager::MatchManager(int normalSpeed, 
                           int fastSpeed, 
                           Texture2D background, 
                           const std::string& address, 
                           int port_host): net(std::make_shared<NetworkManager>()),
                                           player1(normalSpeed, fastSpeed, background, net, address, port_host),
                                           player2(nullptr)
{}

void MatchManager::init(){
    player1.initNetwork();
    player1.connectToServer();
}

void MatchManager::createRemoteGame(UsernamePacket packet){
    if (player2 != nullptr) {
        delete player2;
    }

    std::string name = packet.username;
    player2 = new GameRemote(normalSpeed, fastSpeed, background, 700, 25, packet.id);
    player2.setUsername(name);
}

void matchManager::update() {
    Packet packet = net->readNetwork();

    GamePacket packet;
    
    while(net->pollPacket(packet)){
        switch(packet.type){
            case MessageType::GRID:{
                if(packet.data.size()>=sizeof(GridPacket)){
                    GridPacket *gridPkt = reinterpret_cast<GridPacket*>(packet.data.data());
                    player2.setRemoteGrid(*gridPkt);
                }
                break;
            }
            case MessageType::STATS:{
                if(packet.data.size()>=sizeof(StatPacket)){
                    StatPacket *statPkt = reinterpret_cast<StatPacket*>(packet.data.data());
                    player2.setRemoteStat(*statPkt);
                }
                break;
            }
            case MessageType::BLOCK:{
                if(packet.data.size()>=sizeof(BlockPacket)){
                    BlockPacket *blockPkt = reinterpret_cast<BlockPacket*>(packet.data.data());
                    player2.setRemoteBlock(*blockPkt);
                }
                break;
            }
            case MessageType::SET_ID:{
                if(packet.data.size()>=sizeof(SetIdPacket)){
                    SetIdPacket *idPkt = reinterpret_cast<SetIdPacket*>(packet.data.data());
                    player1.setID(*idPkt);
                }
                break;
            }
            case MessageType::USERNAME:{
                if(packet.data.size()>=sizeof(UsernamePacket)){
                    UsernamePacket *namePkt = reinterpret_cast<UsernamePacket*>(packet.data.data());
                    player2.createRemoteGame(*namePkt);
                }
                break;
            }
        }
    }
    
    player1.Update();

    //if (player1.isGameOver()) sendDefeat();
}

void MatchManager::draw(){
    player1.draw();
    player2.draw();
}

MatchManager::~MatchManager(){
    if (player2 != nullptr) {
        delete player2;
        player2 = nullptr;
    }
}