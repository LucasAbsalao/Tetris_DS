#include <raylib.h>
#include<iostream>
#include "game.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

using namespace std;

int main(){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"raylib Tetris");
    SetTargetFPS(60);

    Grid grid(10,24,20,1);
    grid.generateBlock();

    int count = 0;
    int limit = 60;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLUE);
        grid.draw();
        grid.update(count, limit);
        DrawText("Congrats! You created your first window!", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
}