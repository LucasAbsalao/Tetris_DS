#pragma once

#include<enet/enet.h>
#include<iostream>
#include<map>
#include<cstdint>
#include<queue>
#include <mutex>
#include <thread>
#include <atomic>

#include"networkStructures.hpp"
#include"networkExceptions.hpp"

class NetworkManager{
    private:
        // ENet specific structures for connection management
        ENetAddress address;
        ENetEvent event;
        ENetPeer *peer;
        ENetHost *client;

        // Unique identifier for this client
        int id;

        // Thread-safe buffer to store incoming packets until the main game loop processes them
        std::queue<GamePacket> packetQueue;

        // --- Concurrency Control ---
        
        // Prevents race conditions when pushing to (Network Thread) or popping from (Main Thread) the queue
        std::mutex queueMutex; 

        // Prevents the Main Thread (sending) and Network Thread (receiving) from accessing ENet simultaneously
        std::mutex enetMutex;  

        // Atomic flag to control the lifecycle of the background thread
        std::atomic<bool> shouldRun; 
        
        // The background thread instance that runs the receive loop
        std::thread netThread;

    public:
        // Constructor: Initializes default values
        NetworkManager();

        // Destructor: Ensures the thread is stopped and ENet resources are cleaned up
        ~NetworkManager();

        // Spawns the background thread which executes readNetwork()
        void startThread();

        // Signals the thread to stop and waits for it to finish (joins)
        void stopThread();

        // Initializes the global ENet library (must be called once)
        void initEnet();

        // Creates the local ENet host (client)
        void initClient(int port);

        // Resolves the hostname and attempts to establish a connection to the server
        void connectToServer(const char *ip_address, int port);

        // Low-level function to wrap data into an ENet packet and send it
        // This function is thread-safe (locks enetMutex)
        void sendPacket(const void *data, int size, PacketType flag = PacketType::Reliable);

        // The main loop for the background thread.
        // Continuously polls ENet for events and pushes received data into the packetQueue
        void readNetwork();

        // Called by the Main Thread to retrieve the next available packet from the queue
        // Returns true if a packet was retrieved, false if the queue was empty
        bool pollPacket(GamePacket& outPacket);

        // Template helper to serialize and send a C++ struct over the network
        template<typename T>
        void sendStruct(T data, PacketType flag = PacketType::Reliable){
            sendPacket(&data, sizeof(T), flag);
        }
};