#include<iostream>
#include<enet/enet.h>
#include"server.hpp"

int main(){
    Server server;
    std::cout << "Server created\n";

    try{
        server.initEnet();
        std::cout << "Enet Initialized.\n";
        server.initServer(7788);
        std::cout << "All ready, let's start running the server!!!\n";
        server.run();
    }
    catch (const EnetException& e) {
        std::cerr << "[NetworkError] " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[CRASH] " << e.what() << "\n";
    }
    
    return 0;
}