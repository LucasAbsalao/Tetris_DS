#pragma once

#include"gameMultiplayer.hpp"
#include"gameRemote.hpp"
#include"networkManager.hpp"
#include"networkStructures.hpp"

/* This class is responsible for:
 * 1. Managing the lifecycle of the match (Lobby -> Playing -> End).
 * 2. Owns the Local Player (Logic) and the Remote Player (Replica).
 * 3. Dispatches network packets received from NetworkManager to the appropriate game objects.
 * 4. Determines the win/loss conditions.
 */
class MatchManager {
    private:
        // Configuration backups to reset the game state if needed
        int savedNormalSpeed;
        int savedFastSpeed;
        Texture2D savedBackground;

        // --- State Flags ---
        bool start; // True if the match is currently running (both players ready)
        bool win;   // True if the local player has won
        bool end;   // True if the match has finished (Win or Loss)

        // --- Core Components ---
        std::shared_ptr<NetworkManager> net; // Handles low-level packet I/O
        
        GameMultiplayer player1; // The LOCAL player (handles input, physics, and sending data)

        // The REMOTE opponent (passive, updated via network packets). Pointer because it's created dynamically upon connection.
        // Using pointer can also allow further expansion to more than 1 remote multiplayer;
        GameRemote *player2;    

    public:

        //Sets up the local player and network settings, but does not start the match immediately.
        MatchManager(int normalSpeed, int fast_speed, const std::string& address, int port_host);

        //Responsible for cleaning up the remote player (if allocated) and closing the network thread.
        ~MatchManager();

        // --- Lobby & Synchronization Methods ---

        // Checks if both the local and remote players have set their 'ready' flag.
        // True if the match can begin.
        bool allPlayersAreReady();

        // reads the packet Type (GRID, BLOCK, ATTACK, etc.) and routes the data to `player2->setRemote...()` or `player1`.
        void handlePacket();

        //If allPlayersAreReady() is true, sets the 'start' flag to true.
        void checkStartMatch();


        //Sends the PLAYER_READY packet to the server to notify the opponent.
        void readyToStart();

        //Connects to the host and starts the listening thread.
        void init();


        //Called usually when a USERNAME packet is received, confirming an opponent exists.
        void createRemoteGame(UsernamePacket packet);

        // Sets the display name for the local player (to be sent to the opponent).
        void setLocalPlayerName(const std::string& name);

        // --- Game Loop Methods ---

        //  Main execution of MatchManager, responsible for executing local and remote multiplayer game
        void run();


        //Checks if the local player lost (GameOver) and has more points than the remote one
        void checkWin();


        //Updates the 'end' flag based on win/loss status.
        void checkEnd();
        
        // --- Getters ---

        bool getWin() const;
        bool getEnd() const;
        
        // Accessors for the UI/Renderer to draw the grids/stats
        const GameMultiplayer& getPlayer1() const;
        GameRemote* getPlayer2() const;
};