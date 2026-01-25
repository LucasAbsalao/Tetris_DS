#pragma once

#include<enet/enet.h>
#include<iostream>
#include<map>
#include<cstdint>
#include<queue>
#include <mutex>
#include <thread>
#include <atomic>

#include"network/clientData.hpp"
#include"network/networkStructures.hpp"
#include"network/networkExceptions.hpp"

class NetworkManager{
    private:
        ENetAddress address;
        ENetEvent event;
        ENetPeer *peer;
        ENetHost *client;

        std::map<int, ClientData*> client_map;

        int id;
        std::queue<GamePacket> packetQueue;

        //Concurrence
        std::mutex queueMutex; // Protects packetQueue
        std::mutex enetMutex;  // Protects ENET calls

        std::atomic<bool> shouldRun; // Flag to start and stop the thread
        std::thread netThread;       // The thread

    public:
        NetworkManager();
        ~NetworkManager();
        void startThread();
        void stopThread();
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