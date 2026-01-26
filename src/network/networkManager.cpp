#include"networkManager.hpp"

NetworkManager::NetworkManager(): id(-1)
{}

NetworkManager::~NetworkManager(){

    //Resource Acquisition Is Initialization
    stopThread();

    std::cout << "\n\n----------------------- Deleting the client -----------------------\n";

    if(client != nullptr && peer != nullptr) {
        std::cout << "pre disconnect" << client << " " << peer << "\n";

        enet_peer_disconnect(peer, 0);
        std::cout << "post disconnect";

        ENetEvent event;

        bool disconnected = false;
        while(enet_host_service(client, &event, 500) > 0) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                enet_packet_destroy(event.packet);
            } 
            else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                std::cout << "Disconnection succeeded!\n";
                disconnected = true;
                this->peer = nullptr;
                break;
            }
        }
        std::cout << "Pos analaise dos pacotes\n";

        if(!disconnected) {
            std::cout << "Forcing peer reset...\n";
            enet_peer_reset(peer);
        }
    }

    if(client != nullptr) {
        enet_host_destroy(client);
        client = nullptr;
    }
}

void NetworkManager::initEnet(){
}

void NetworkManager::initClient(int port){
    client = enet_host_create(NULL,
                              1,
                              2,
                              0,
                              0);
    if(client == NULL){
        throw EnetException("Could not create an Enet Host.");
    }
}

void NetworkManager::connectToServer(const char *ip_address, int port){
    if (enet_address_set_host(&address, ip_address) != 0) { // Setting Host Address
        throw EnetException("Could not resolve the address");
        return; 
    }
    address.port = port;

    std::cout << address.host << ":" << address.port << "\n";

    
    peer = enet_host_connect(client, &address, 2, 0); // Connecting to Host with 2 channels (a reliable one to grid and stats | an unreliable to block)
    if(peer==NULL){
        throw EnetException("No available peers for initiating an ENet connection\n");
    }

    int eventStatus = enet_host_service(client, &event, 5000);

    if (eventStatus > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "SUCESSO! Conectado ao servidor.\n";
        } else {
            std::cout << "Strange: A non connect event was captures. Type: " << event.type << "\n";
        }
    } else if (eventStatus == 0) {
        enet_peer_reset(peer);
        throw ServerConnectionException();
    } else {
        throw EnetException("Fail in the enet_host_service function.\n");
    }

    //Starting Thread
    startThread();
}

void NetworkManager::startThread() {
    shouldRun = true;
    netThread = std::thread(&NetworkManager::readNetwork, this);
}

void NetworkManager::stopThread() {
    shouldRun = false;
    if (netThread.joinable()) {
        netThread.join();
    }
}

void NetworkManager::readNetwork(){
    while(shouldRun){
        int serviceResult = 0;
        //Lock Mutex so that the main thread can't send informations at the same time
        {
            std::lock_guard<std::mutex> lock(enetMutex); 
            serviceResult = enet_host_service(client, &event, 10);
        }

        if(serviceResult<=0){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        switch(event.type){
            case ENET_EVENT_TYPE_RECEIVE:{
                if(event.packet->dataLength > 0){
                    uint8_t *rawData = static_cast<uint8_t*>(event.packet->data);
                    MessageType type = static_cast<MessageType>(rawData[0]);

                    GamePacket gp;
                    gp.type = type;

                    gp.data.assign(rawData, rawData + event.packet->dataLength);
                    
                    //Lock the Queue mutex to prevent the MultiplayerGame to pop informations at the same time
                    {
                        std::lock_guard<std::mutex> queueLock(queueMutex);
                        packetQueue.push(gp);
                    }

                }

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:{
                std::cout << "Disconnecting from the server\n";
                break;
            }
        }
    }
}

bool NetworkManager::pollPacket(GamePacket& outPacket) {
    //Lock the queue to extract informations;
    std::lock_guard<std::mutex> lock(queueMutex);
    if (packetQueue.empty()) {
        return false; 
    }

    outPacket = packetQueue.front();
    packetQueue.pop();
    return true; 
}

void NetworkManager::sendPacket(const void *data, int size, PacketType flag){
    int enetFlag = static_cast<int>(flag);

    ENetPacket *packet = enet_packet_create(data, size, enetFlag);

    int channel = (flag == PacketType::Reliable) ? 1 : 0;

    //Locking the enet mutex so that enet_host_service is not called at the same time as enet_peer_send
    {
        std::lock_guard<std::mutex> lock(enetMutex);
        enet_peer_send(peer, channel, packet);
    }
}