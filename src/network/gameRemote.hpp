#pragma once

#include "../core/game.hpp"
#include"networkStructures.hpp"
#include<string>

class GameRemote : public Game {
    private:
        uint8_t id_client;      // Unique ID of this remote player (to match incoming packets)
        std::string username;   // Display name of the opponent
        bool readyToStart;      // Flag indicating if the opponent is ready
        bool connected;         // Connectivity status (true = online, false = disconnected)

    public:

        //Constructor for the Remote Game.
        GameRemote(int normalSpeed, int fastSpeed, int position_x, int position_y, uint8_t id_client);

        // --- Logic Overrides (Disabled or Passive) ---
        // These methods are overridden to prevent the local physics engine 
        // from moving the remote player's pieces autonomously.
        virtual void run() override;
        virtual void moveBlock(char direction) override;
        virtual void rotateBlock() override;
        virtual void insertBlockInGrid() override;
        virtual void updateGridStat(int count_lines, int completed_line) override;

        // --- Network State Setters (The "Active" Logic) ---

        // Marks the player as disconnected (can trigger UI changes like "Connection Lost").
        void wasDisconnected(); 

        // Sets the opponent's display name received from the server.
        void setUsername(std::string name);

        // Updates the ready state based on a PLAYER_READY packet.
        void setRemoteReady(ReadyPacket packet);

        // Triggers the Game Over state for the opponent based on a GAME_OVER packet.
        void setRemoteGameOver(GameOverPacket packet);
       
        //  Called when a BLOCK packet is received. Forces the remote block 
        //  to match the position, rotation, and type sent by the server.
        void setRemoteBlock(BlockPacket packet);


        // Called when a GRID packet is received. Completely overwrites 
        // the local copy of the opponent's grid with the server data.
        void setRemoteGrid(GridPacket packet);

        // Updates score, level, and lines based on a STATS packet.
        void setRemoteStat(StatPacket packet);

        // --- Getters ---
        
        uint8_t getId() const;
        bool getConnected() const;
        bool getReadyToStart() const;
        const std::string& getUsername() const;
};
