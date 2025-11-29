#include <raylib.h>
#include<iostream>
#include "game.hpp"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 720
#define NORMAL_SPEED 15
#define FAST_SPEED 2

using namespace std;

int main(){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"raylib Tetris");
    SetTargetFPS(60);

    Grid grid(10,24,25,{125,65},0);
    grid.generateBlock();

    int count = 0;
    int limit = NORMAL_SPEED;
    bool goDown = 0;
    int score = 0;
    char str_score[50];

    while(!WindowShouldClose()){
        BeginDrawing();

        DrawText("TETRIS", 145, 10, 50, {0,0,0,255});
        sprintf(str_score, "Score: %d", score);
        DrawText(str_score, 400, 50, 30, {0,0,0,255});
        ClearBackground(VIOLET);

        if(IsKeyDown(KEY_DOWN) && !goDown){
            limit = FAST_SPEED;
            goDown = true;
        }
        else if (IsKeyUp(KEY_DOWN) && goDown){
            limit = NORMAL_SPEED;
            count = 0;
            goDown = false;
        }

        grid.getMovement();
        grid.draw();
        grid.update(count, limit);

        int x = grid.getCompletedLine(score);

        EndDrawing();
    }

    CloseWindow();
}