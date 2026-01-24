#include<iostream>
#include<enet/enet.h>
#include"server.hpp"

int main(){
    Server server;
    std::cout << "Server created\n";
    server.initEnet();
    std::cout << "Enet Initialized.\n";
    server.initServer(7788);

    std::cout << "All ready, let's start running the server!!!\n";
    server.run();
    
    return 0;
}