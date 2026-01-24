#include "gameMultiplayer.hpp"

GameMultiplayer::GameMultiplayer(int normalSpeed, int fast_speed, Texture2D background): Game(normalSpeed, fast_speed, background),
                                                                                         net(std::make_shared<NetworkManager>()),
                                                                                         address_host("127.0.0.1"),
                                                                                         port_host(7777),
                                                                                         id_client(-1)
{}

GameMultiplayer::GameMultiplayer(int normalSpeed, 
                                 int fast_speed, 
                                 Texture2D background, 
                                 std::shared_ptr<NetworkManager> ptr_net,
                                 const std::string& address, 
                                 int port_host): Game(normalSpeed, fast_speed, background), 
                                                 net(ptr_net),
                                                 address_host(address),
                                                 port_host(port_host),
                                                 id_client(-1)
{} 

void GameMultiplayer::initNetwork(){ // TODO: Exception
    std::cout << "Multiplayer: Initializing Enet\n";
    net->initEnet();
    std::cout << "Multiplayer: Initializing Client\n";
    net->initClient(port_host);
    std::cout << "Multiplayer: Initialization completed\n";
}

void GameMultiplayer::connectToServer(){
    std::cout << "Multiplayer: Attempting to connect to host " << address_host << ":" << port_host << "\n";
    net->connectToServer(address_host.c_str(), port_host);
    std::cout << "Multiplayer: Connection completed\n";
}

void GameMultiplayer::run(){
    //net->readingMsg();
    Game::run();
}

void GameMultiplayer::insertBlockInGrid(){
    Game::insertBlockInGrid();
    GridPacket packet = getGrid().toPacket(id_client);
    net->sendStruct(packet);
}

void GameMultiplayer::rotateBlock(){
    Game::rotateBlock();
    BlockPacket packet = getBlock().toPacket(id_client);
    net->sendStruct(packet, PacketType::Unreliable);
}

void GameMultiplayer::updateGridStat(int count_lines, int completed_line){
    Game::updateGridStat(count_lines, completed_line);
    StatPacket packetStat = getStat().toPacket(id_client);
    GridPacket packetGrid = getGrid().toPacket(id_client);
    net->sendStruct(packetStat);
    net->sendStruct(packetGrid);
}

void GameMultiplayer::moveBlock(char direction){
    Game::moveBlock(direction);
    BlockPacket packet = getBlock().toPacket(id_client);
    net->sendStruct(packet, PacketType::Unreliable);
}

void GameMultiplayer::setAddress(std::string addressIP){
    address_host = addressIP;
}

void GameMultiplayer::setPortHost(int portServer){
    port_host = portServer;
}

void GameMultiplayer::setID(SetIdPacket packet){
    this->id_client = packet.id;
}