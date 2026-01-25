#include "graphics/raylib_app.hpp"

int main() {
    try {
        RaylibApp app(1400, 720, 15, 2, "127.0.0.1", 7788);
        app.run();
    } 
    catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
