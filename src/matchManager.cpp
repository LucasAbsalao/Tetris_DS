#include "matchManager.hpp"

MatchManager::MatchManager(int normalSpeed, 
                           int fastSpeed, 
                           const std::string& address, 
                           int port_host): net(std::make_shared<NetworkManager>()),
                                           player1(normalSpeed, fastSpeed, net, address, port_host),
                                           player2(nullptr),
                                           start(false),
                                           savedNormalSpeed(normalSpeed),
                                           savedFastSpeed(fastSpeed)
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
    player2 = new GameRemote(savedNormalSpeed, savedFastSpeed, 700, 65, packet.id);
    player2->setUsername(name);
    std::cout<<"Creating a new game with id " << packet.id << "and name " << name << "\n";
}

bool MatchManager::allPlayersAreReady(){
    return (player1.getReadyToStart() && player2->getReadyToStart());
}

void MatchManager::checkStartMatch(){
    if(allPlayersAreReady()){
        start = true;
    }
    else{
        start = false;
    }
}

void MatchManager::handlePacket(){
    //net->readNetwork();

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
            case MessageType::GAME_OVER:{
                if(player2 != nullptr && packet.data.size()>=sizeof(GameOverPacket)){
                    GameOverPacket *gameoverPkt = reinterpret_cast<GameOverPacket*>(packet.data.data());
                    if(player2->getId() == gameoverPkt->id){
                        player2->setRemoteGameOver(*gameoverPkt);
                        std::cout << "Player " << player2->getUsername() << " lost!!!" << "\n";
                    }
                }
                break;
            }
            case MessageType::PLAYER_READY:{
                if(player2 != nullptr && packet.data.size()>=sizeof(ReadyPacket)){
                    ReadyPacket *readyPkt = reinterpret_cast<ReadyPacket*>(packet.data.data());
                    if(player2->getId() == readyPkt->id)
                        player2->setRemoteReady(*readyPkt);
                }
                break;
            }
            case MessageType::PLAYER_DISCONNECTED:{
                if(player2 != nullptr && packet.data.size()>=sizeof(DisconnectPacket)){
                    DisconnectPacket *disconPkt = reinterpret_cast<DisconnectPacket*>(packet.data.data());
                    if(player2->getId() == disconPkt->id)
                        std::cout << "Player " << player2->getUsername() << " Disconnected\n";
                }
                break;
            }
            case MessageType::ATTACK:{
                if(player2 != nullptr && packet.data.size()>=sizeof(AttackPacket)){
                    AttackPacket *attackPkt = reinterpret_cast<AttackPacket*>(packet.data.data());
                    if(player2->getId() == attackPkt->id)
                        player1.receiveAttack(attackPkt->lines);
                }
                break;
            }
        }
    }
}

void MatchManager::run() {
    handlePacket();
    
    if(start){
        player1.run();
    }
    else{
        checkStartMatch();
    }
}

void MatchManager::setLocalPlayerName(const std::string& name) {
    player1.setUsername(name);
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

const GameMultiplayer& MatchManager::getPlayer1() const { 
    return player1; 
}

GameRemote* MatchManager::getPlayer2() const { 
    return player2; 
}