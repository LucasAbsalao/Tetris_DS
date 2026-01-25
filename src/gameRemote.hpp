#pragma once

#include "game.hpp"
#include"network/networkStructures.hpp"
#include<string>

class GameRemote : public Game {
    private:
        uint8_t id_client;
        std::string username;
        bool readyToStart;
        bool connected;

    public:
        GameRemote(int normalSpeed, int fastSpeed, int position_x, int position_y, uint8_t id_client);
        virtual void run() override;
        virtual void moveBlock(char direction) override;
        virtual void rotateBlock() override;
        virtual void insertBlockInGrid() override;
        virtual void updateGridStat(int count_lines, int completed_line) override;


        void wasDisconnected(); 
        void setUsername(std::string name);
        void setRemoteReady(ReadyPacket packet);
        void setRemoteGameOver(GameOverPacket packet);
        void setRemoteBlock(BlockPacket packet);
        void setRemoteGrid(GridPacket packet);
        void setRemoteStat(StatPacket packet);

        uint8_t getId() const;
        bool getConnected() const;
        bool getReadyToStart() const;
        const std::string& getUsername() const;
};
