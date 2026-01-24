#pragma once

#include<enet/enet.h>
#include<iostream>
#include<map>
#include<cstdint>
#include<queue>

#include"network/clientData.hpp"
#include"network/networkStructures.hpp"

class NetworkManager{
    private:
        ENetAddress address;
        ENetEvent event;
        ENetPeer *peer;
        ENetHost *client;
        std::map<int, ClientData*> client_map;
        int id;
        std::queue<GamePacket> packetQueue;

    public:
        NetworkManager();
        ~NetworkManager();
        void initEnet();
        void initClient(int port);
        void connectToServer(const char *ip_address, int port);
        void sendPacket(const void *data, int size, PacketType flag = PacketType::Reliable);
        void handlePacket();
        void readNetwork();
        bool pollPacket(GamePacket& outPacket);

        template<typename T>
        void sendStruct(T data, PacketType flag = PacketType::Reliable){
            sendPacket(&data, sizeof(T), flag);
        }
};