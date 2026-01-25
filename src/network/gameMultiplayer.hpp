#pragma once

#include "../core/game.hpp"
#include "networkManager.hpp"
#include"networkStructures.hpp"
#include<string>

class GameMultiplayer : public Game {
    private:
        std::shared_ptr<NetworkManager> net;
        std::string address_host;
        int port_host;
        std::string username;
        uint8_t id_client;
        bool readyToStart;

    public:
        GameMultiplayer(int normalSpeed, int fast_speed);
        GameMultiplayer(int normalSpeed, int fast_speed, std::shared_ptr<NetworkManager> ptr_net, const std::string& address, int port_host);
        void initNetwork();
        void connectToServer();
        virtual void run() override;
        virtual void moveBlock(char direction) override;
        virtual void rotateBlock() override;
        virtual void insertBlockInGrid() override;
        virtual void updateGridStat(int count_lines, int completed_line) override;
        virtual void setGameOver() override;
        virtual void receiveAttack(int lines) override;
        
        int getId();
        bool getReadyToStart();

        void setAddress(std::string addressIP);
        void setPortHost(int portServer);
        void setID(SetIdPacket packet);
        void setUsername(std::string name);
        void setReady(bool ready);
};