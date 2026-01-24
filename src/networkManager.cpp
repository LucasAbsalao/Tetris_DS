#include"networkManager.hpp"

NetworkManager::NetworkManager(): id(-1)
{}

NetworkManager::~NetworkManager(){
    std::cout << "\n\n----------------------- Deleting the client -----------------------\n";
    for(auto const& x:client_map){
        std::cout << "Deleting the client " << x.second << "\n";
        delete x.second;
    }

    client_map.clear();

    std::cout<< "All clients deleted\n" << "Asking for disconnection to the server\n";

    if(client!=nullptr && peer != nullptr){
        enet_peer_disconnect(peer, 0);

        while(enet_host_service(client, &event, 5000)>0){
            switch(event.type){
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    std::cout << "Disconnection succeeded!\n";
                    break;
                }
            }
        }
        enet_peer_reset (peer);
    }//TODO: Do something if the disconnection fails.
}

void NetworkManager::initEnet(){
    if(enet_initialize()!=0){ //TODO: Exception
        std::cout << "An error occurred while initializing ENet\n";
    }
    atexit(enet_deinitialize);
}

void NetworkManager::initClient(int port){
    client = enet_host_create(NULL,
                              1,
                              2,
                              0,
                              0);
    if(client == NULL){ //TODO: Exception
        std::cout << "An error accurred while trying to create an ENet client host\n";
    }
}

void NetworkManager::connectToServer(const char *ip_address, int port){
    enet_address_set_host(&address, ip_address); // Setting Host Address
    address.port = port;

    std::cout << address.host << ":" << address.port << "\n";

    peer = enet_host_connect(client, &address, 2, 0); // Connecting to Host with 2 channels (a reliable one to grid and stats | an unreliable to block)
    if(peer==NULL){ //TODO: Exception
        fprintf(stderr, "No available peers for initiating an ENet connection\n");
    }

    int eventStatus = enet_host_service(client, &event, 5000);

    if (eventStatus > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "SUCESSO! Conectado ao servidor.\n";
        } else {
            std::cout << "ESTRANHO: Recebi um evento, mas não foi CONNECT. Tipo: " << event.type << "\n";
        }
    } else if (eventStatus == 0) {
        std::cout << "TIMEOUT: O servidor não respondeu em 5 segundos.\n";
        enet_peer_reset(peer);
    } else {
        std::cout << "ERRO: Falha na função enet_host_service.\n";
    }
}

void NetworkManager::readNetwork(){
    while(enet_host_service(client, &event, 0)>0){
        switch(event.type){
            case ENET_EVENT_TYPE_RECEIVE:{
                printf("A packet of length %lu was received from %x:%u on channel %d.\n",
                        event.packet->dataLength,
                        event.peer->address.host,
                        event.peer->address.port,
                        event.channelID);

                if(event.packet->dataLength > 0){
                    uint8_t *rawData = static_cast<uint8_t*>(event.packet->data);
                    MessageType type = static_cast<MessageType>(rawData[0]);

                    GamePacket gp;
                    gp.type = type;

                    gp.data.assign(rawData, rawData + event.packet->dataLength);

                    packetQueue.push(gp);
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
    enet_peer_send(peer, channel, packet);
}