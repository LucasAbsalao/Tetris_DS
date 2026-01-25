#include "raylib_app.hpp"

int main() {
    RaylibApp app(1400, 720, 15, 2, "127.0.0.1", 7788);
    app.run();
    return 0;
}
