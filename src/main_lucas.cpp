#include <raylib.h>
#include<iostream>
#include<string>
#include "game_lucas.hpp"

#define SCREEN_WIDTH 650
#define SCREEN_HEIGHT 720
#define NORMAL_SPEED 15
#define FAST_SPEED 2

using namespace std;

enum GameState {
    START_SCREEN,
    PLAYING,
    INSTRUCTIONS,
    EXITING
};

GameState gameState = START_SCREEN;

void DrawStartScreen(Rectangle startButton, Rectangle instructionsButton, Rectangle exitButton) {
    // Desenho de Fundo e Título
    ClearBackground(RAYWHITE);
    DrawText("RAYLIB TETRIS", SCREEN_WIDTH / 2 - MeasureText("RAYLIB TETRIS", 50) / 2, 100, 50, DARKBLUE);
    
    DrawRectangleRec(startButton, LIGHTGRAY);
    DrawRectangleLinesEx(startButton, 2, DARKGRAY);
    DrawText("START GAME", startButton.x + startButton.width / 2 - MeasureText("START GAME", 20) / 2, startButton.y + startButton.height / 2 - 10, 20, BLACK);

    DrawRectangleRec(instructionsButton, LIGHTGRAY);
    DrawRectangleLinesEx(instructionsButton, 2, DARKGRAY);
    DrawText("INSTRUÇÕES", instructionsButton.x + instructionsButton.width / 2 - MeasureText("INSTRUÇÕES", 20) / 2, instructionsButton.y + instructionsButton.height / 2 - 10, 20, BLACK);

    DrawRectangleRec(exitButton, LIGHTGRAY);
    DrawRectangleLinesEx(exitButton, 2, DARKGRAY);
    DrawText("SAIR", exitButton.x + exitButton.width / 2 - MeasureText("SAIR", 20) / 2, exitButton.y + exitButton.height / 2 - 10, 20, BLACK);

    if (gameState == INSTRUCTIONS) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(RAYWHITE, 0.9f));
        DrawText("INSTRUÇÕES DO JOGO:", 50, 100, 30, DARKGRAY);
        DrawText("Use as setas para mover e girar.", 50, 150, 20, DARKGRAY);
        DrawText("Pressione 'Enter' para voltar ao menu.", 50, 200, 20, DARKGRAY);
    }
}

int main(){
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"raylib Tetris");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/background.jpeg");
    Game game(NORMAL_SPEED, FAST_SPEED, background);

    Rectangle startButton = {SCREEN_WIDTH / 2 - 100, 300, 200, 50};
    Rectangle instructionsButton = {SCREEN_WIDTH / 2 - 100, 370, 200, 50};
    Rectangle exitButton = {SCREEN_WIDTH / 2 - 100, 440, 200, 50};

    while (!WindowShouldClose() && gameState != EXITING) {
        
        if (gameState == START_SCREEN) {
            Vector2 mousePoint = GetMousePosition();
            bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            
            if (mouseClicked) {
                if (CheckCollisionPointRec(mousePoint, startButton)) {
                    gameState = PLAYING;
                } else if (CheckCollisionPointRec(mousePoint, instructionsButton)) {
                    gameState = INSTRUCTIONS; 
                } else if (CheckCollisionPointRec(mousePoint, exitButton)) {
                    gameState = EXITING; 
                }
            }
        } 
        
        else if (gameState == INSTRUCTIONS) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                gameState = START_SCREEN;
            }
        }

        if (WindowShouldClose()) {
            gameState = EXITING;
        }


        BeginDrawing();

        if (gameState == PLAYING) {
            game.run();
            game.draw(); 
        } else {
            DrawStartScreen(startButton, instructionsButton, exitButton);
        }

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}