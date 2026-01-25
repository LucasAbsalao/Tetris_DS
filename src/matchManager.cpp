#include "matchManager.hpp"

MatchManager::MatchManager(int normalSpeed, 
                           int fastSpeed, 
                           Texture2D background, 
                           const std::string& address, 
                           int port_host): net(std::make_shared<NetworkManager>()),
                                           player1(normalSpeed, fastSpeed, background, net, address, port_host),
                                           player2(nullptr),
                                           savedNormalSpeed(normalSpeed),
                                           savedFastSpeed(fastSpeed),
                                           savedBackground(background)
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
    player2 = new GameRemote(savedNormalSpeed, savedFastSpeed, savedBackground, 700, 25, packet.id);
    player2->setUsername(name);
    std::cout<<"Creating a new game with id " << packet.id << "and name " << name << "\n";
}

void MatchManager::run() {
    net->readNetwork();

    GamePacket packet;
    
    while(net->pollPacket(packet)){
        switch(packet.type){
            case MessageType::GRID:{
                if(player2 != nullptr && packet.data.size()>=sizeof(GridPacket)){
                    GridPacket *gridPkt = reinterpret_cast<GridPacket*>(packet.data.data());
                    if(player2->getId() == gridPkt->id)
                        player2->setRemoteGrid(*gridPkt);
                }
                break;
            }
            case MessageType::STATS:{
                if(player2 != nullptr && packet.data.size()>=sizeof(StatPacket)){
                    StatPacket *statPkt = reinterpret_cast<StatPacket*>(packet.data.data());
                    if(player2->getId() == statPkt->id)
                        player2->setRemoteStat(*statPkt);
                }
                break;
            }
            case MessageType::BLOCK:{
                if(player2 != nullptr && packet.data.size()>=sizeof(BlockPacket)){
                    BlockPacket *blockPkt = reinterpret_cast<BlockPacket*>(packet.data.data());
                    if(player2->getId() == blockPkt->id)
                        player2->setRemoteBlock(*blockPkt);
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
                    if(namePkt->id != player1.getId())
                        createRemoteGame(*namePkt);
                }
                break;
            }
        }
    }
    
    player1.run();

    //if (player1.isGameOver()) sendDefeat();
}

void MatchManager::setLocalPlayerName(const std::string& name) {
    player1.setUsername(name);
}

void MatchManager::draw(){
    player1.draw();
    if (player2 != nullptr) {
        player2->draw();
    }
}

void MatchManager::readyToStart(){
    player1.setReady(true);
}


MatchManager::~MatchManager(){
    if (player2 != nullptr) {
        delete player2;
        player2 = nullptr;
    }
}
