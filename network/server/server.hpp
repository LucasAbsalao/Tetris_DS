#pragma once

#include<iostream>
#include<stdio.h>
#include<enet/enet.h>
#include<string>
#include<vector>
#include<cstring>
#include<map>

#include"../../src/network/clientData.hpp"
#include"../../src/network/networkExceptions.hpp"
#include"../../src/network/networkStructures.hpp"

class Server{
    private:
        std::map<int, ClientData*> client_map;
        ENetAddress address;
        ENetEvent event;
        ENetHost *server;
        uint8_t id_new_client;


    public:
        Server();
        ~Server();
        void initEnet();
        void initServer(int port);
        void sendPacket(ENetPeer *peer, const void *data, size_t size, PacketType flag = PacketType::Reliable);
        void broadcastPacket(const void *data, size_t size, PacketType flag = PacketType::Reliable);
        void run();
        void parseMessage(int sender_id, void *data);
        void handlePacket();

        template <typename T>
        void sendStruct(ENetPeer *peer, const T& data, PacketType flag = PacketType::Reliable){
            sendPacket(peer, &data, sizeof(T), flag);
        }

        template <typename T>
        void broadcastStruct(const T& data, PacketType flag = PacketType::Reliable){
            broadcastPacket(&data, sizeof(T), flag);
        }
};