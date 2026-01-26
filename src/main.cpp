#include "graphics/raylib_app.hpp"
#include<fstream>

int main() {

    std::string ip = "127.0.0.1";
    int port = 7788;

    std::ifstream configFile("config.txt");
    if (configFile) { // Forma mais simples de checar se está aberto e ok
        std::cout << "Configuração carregada com sucesso!" << std::endl;
        configFile >> ip >> port;
        configFile.close();
    } else {
        std::cerr << "config.txt not found\n" << "Using standard values\n";
    }


    try {
        RaylibApp app(1400, 720, 15, 2, ip, port);
        app.run();
    } 
    catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
