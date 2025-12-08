#include <raylib.h>
#include<iostream>
#include<string>
#include "game_lucas.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define NORMAL_SPEED 15
#define FAST_SPEED 2

using namespace std;

int main(){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"raylib Tetris");
    SetTargetFPS(60);

    // int count = 0;
    // int limit = NORMAL_SPEED;
    // bool goDown = 0;
    // int score = 0;
    // char str_score[50];

    Texture2D background = LoadTexture("assets/background.jpeg");
    Game game(NORMAL_SPEED, FAST_SPEED, background);

    while(!WindowShouldClose()){
        BeginDrawing();
        game.run();
        game.draw();
        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
}