#pragma once

#include "../core/game.hpp"
#include "networkManager.hpp"
#include"networkStructures.hpp"
#include<string>

class GameMultiplayer : public Game {
    private:
        std::shared_ptr<NetworkManager> net; // Shared pointer to the network manager
        std::string address_host;            // Server IP address
        int port_host;                       // Server Port
        std::string username;                // Local player's username
        uint8_t id_client;                   // Unique ID assigned by the server to this client
        bool readyToStart;                   // Flag indicating if the player clicked "Ready"

    public:
        // Default constructor (useful for initialization before network setup)
        GameMultiplayer(int normalSpeed, int fast_speed);

        // Main constructor: Injects the NetworkManager instance and connection details
        GameMultiplayer(int normalSpeed, int fast_speed, std::shared_ptr<NetworkManager> ptr_net, const std::string& address, int port_host);

        // Initializes the ENet library and creates the Client Host
        void initNetwork();

        // Attempts to establish a connection (Handshake) with the server at the defined IP/Port
        void connectToServer();

        // Main game loop override.
        // Handles game updates and potentially checks for start conditions (Lobby state).
        virtual void run() override;

        // Override: Moves the block locally AND sends the new position to the server.
        virtual void moveBlock(char direction) override;

        // Override: Rotates the block locally AND sends the rotation update to the server.
        virtual void rotateBlock() override;

        // Override: Locks the block into the grid.
        // It is critical to send the final Grid state here to ensure consistency.
        virtual void insertBlockInGrid() override;

        // Override: Checks for completed lines.
        // If lines are cleared, updates stats and sends an ATTACK packet (garbage lines) to the opponent.
        virtual void updateGridStat(int count_lines, int completed_line) override;

        // Override: Ends the local game and sends a GAME_OVER packet to the server.
        virtual void setGameOver() override;

        // Multiplayer-specific function:
        // Receives "garbage" (gray lines) sent by the opponent and pushes them to the bottom of the grid.
        virtual void receiveAttack(int lines) override;
        
        // Returns the unique ID assigned by the server to this client.
        int getId();

        // Checks if the local player has already signaled they are ready to start.
        bool getReadyToStart();

        // Sets the target Server IP address.
        void setAddress(std::string addressIP);

        // Sets the target Server Port.
        void setPortHost(int portServer);

        // Processes the SET_ID packet received from the server to store the client's identity.
        void setID(SetIdPacket packet);

        // Sets the local username (which will be broadcast to the opponent).
        void setUsername(std::string name);

        // Toggles the ready state and sends the PLAYER_READY packet to the server.
        void setReady(bool ready);

        //Get username
        const std::string& getUsername() const;
};
