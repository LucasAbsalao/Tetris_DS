#include "server.hpp"

Server::Server(): id_new_client(0), server(nullptr)
{}

void Server::initEnet(){
    if(enet_initialize()!=0){ //TODO: Exception
        std::cout << "An error occurred while initializing ENet\n";
    }
    atexit(enet_deinitialize);
}

void Server::initServer(int port){
    address.host = ENET_HOST_ANY;
    address.port = port;
    std::cout << address.host << ":" << address.port << "\n";

    server = enet_host_create(&address,
                              32,
                              2,
                              0,
                              0);
    if(server==NULL){ //TODO:: Exception
        std::cout << "An error occurred while trying to create an ENet Host\n";
    }
}

void Server::sendPacket(ENetPeer *peer, const void *data, size_t size, PacketType flag){
    int enetFlag = static_cast<int>(flag);

    ENetPacket *packet = enet_packet_create(data, size, enetFlag);

    int channel = (flag == PacketType::Reliable) ? 1 : 0;
    enet_peer_send(peer, channel, packet);
}

void Server::broadcastPacket(const void *data, size_t size, PacketType flag){
    int enetFlag = static_cast<int>(flag);

    ENetPacket *packet = enet_packet_create(data, size, enetFlag);

    int channel = (flag == PacketType::Reliable) ? 1 : 0;
    enet_host_broadcast(server, channel, packet);
}

void Server::run(){
    while(true){
        handlePacket();
    }
}

void Server::parseMessage(int sender_id, void *data){ //TODO: Check size
    std::cout << "Parsing Message: ";
    uint8_t *type = reinterpret_cast<uint8_t*>(data);
    
    if(*type == MessageType::USERNAME){
        UsernamePacket *packet = reinterpret_cast<UsernamePacket*>(data);
        packet->id = static_cast<uint8_t>(sender_id);

        if(client_map[packet->id]){
            client_map[packet->id]->setUsername(packet->username);
            std::cout << "Username: " << packet->username << "\n";
            broadcastStruct(*packet);
        }
    }
    else if(*type == MessageType::GRID){
        GridPacket *packet = reinterpret_cast<GridPacket*>(data);
        std::cout << "GRID -> ID: " << static_cast<int>(packet->id) << "\n";
        broadcastStruct(*packet);
    }
    else if(*type == MessageType::BLOCK){
        BlockPacket *packet = reinterpret_cast<BlockPacket*>(data);
        std::cout << "BLOCK -> ID: " << static_cast<int>(packet->id) << "\n";
        broadcastStruct(*packet, PacketType::Unreliable);
    }
    else if(*type == MessageType::STATS){
        StatPacket *packet = reinterpret_cast<StatPacket*>(data);
        std::cout << "STATS -> ID: " << static_cast<int>(packet->id) << "\n";
        broadcastStruct(*packet);
    }
    else if(*type == MessageType::GAME_OVER){
        GameOverPacket *packet = reinterpret_cast<GameOverPacket*>(data);
        std::cout << "GAME_OVER -> ID: " << static_cast<int>(packet->id) << "\n";
        broadcastStruct(*packet);
    }
    else if(*type == MessageType::PLAYER_READY){
        ReadyPacket *packet = reinterpret_cast<ReadyPacket*>(data);
        std::cout << "PLAYER_READY -> ID: " << static_cast<int>(packet->id) << "\n";
        broadcastStruct(*packet);
    }
    else if(*type == MessageType::ATTACK){
        AttackPacket *packet = reinterpret_cast<AttackPacket*>(data);
        std::cout << "ATTACK -> ID: " << static_cast<int>(packet->id) << "\n";
        broadcastStruct(*packet);
    }
}

void Server::handlePacket(){ //TODO: Test if the messageType variable is already initialized the right way
    while(enet_host_service(server, &event, 0)>0){
        switch(event.type){
            case ENET_EVENT_TYPE_CONNECT:{
                std::cout << "A new client is trying to connect from " << event.peer->address.host << ":" << event.peer->address.port << "\n";

                for(auto const &x:client_map){

                    uint8_t client_id = static_cast<uint8_t>(x.first);
                    std::string username = x.second->getUsername();

                    UsernamePacket username_connected_packet;

                    username_connected_packet.type = MessageType::USERNAME;
                    username_connected_packet.id = client_id;

        
                    std::strncpy(username_connected_packet.username, username.data(), sizeof(username_connected_packet.username) - 1);
                    
                    username_connected_packet.username[sizeof(username_connected_packet.username)-1] = '\0'; //Assurance that the last byte of username is always 0
                    sendStruct(event.peer, username_connected_packet);
                }

                client_map[id_new_client] = new ClientData(id_new_client);
                event.peer->data = client_map[id_new_client];

                SetIdPacket idPacket;

                idPacket.type = MessageType::SET_ID;
                idPacket.id = id_new_client;

                if(id_new_client!=255)
                    id_new_client++;
                else 
                    id_new_client = 0;
                sendStruct(event.peer, idPacket);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:{
                // printf("A packet of length %lu was received from %x:%u on channel %d.\n",
                //         event.packet->dataLength,
                //         event.peer->address.host,
                //         event.peer->address.port,
                //         event.channelID);

                parseMessage(static_cast<ClientData*>(event.peer->data)->getId(), event.packet->data);
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:{
                printf("%x:%u disconnected.\n",
                        event.peer->address.host,
                        event.peer->address.port);
                
                DisconnectPacket disconnectPkt;
                
                ClientData *cliData = static_cast<ClientData*>(event.peer->data);
                uint8_t idDisc = static_cast<uint8_t>(cliData->getId());

                delete client_map[(int)idDisc];
                
                client_map.erase(idDisc);

                event.peer->data = nullptr;
                
                disconnectPkt.type = MessageType::PLAYER_DISCONNECTED;
                disconnectPkt.id = idDisc;

                broadcastStruct(disconnectPkt);

                event.peer->data=NULL;
                break;
            }
        }
    }
}

Server::~Server(){
    std::cout << "\n\n ----------------------- Deleting the server -----------------------\n";
    for(auto const& x: client_map){
        std::cout << "Deleting the Client of ID " << x.second->getId();
        delete x.second;
    }

    client_map.clear();
    std::cout << "All the clients were deleted\n";
    if(server!=nullptr)
        enet_host_destroy(server);
    std::cout << "Server deleted successfully\n";
}