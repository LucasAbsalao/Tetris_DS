#include <raylib.h>
#include<iostream>
#include<string>
#include "game.hpp"
#include "gameMultiplayer.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NORMAL_SPEED 15
#define FAST_SPEED 2

using namespace std;

enum GameState {
    START_SCREEN,
    PLAYING,
    PLAYING_MULTIPLAYER,
    INSTRUCTIONS,
    EXITING
};

GameState gameState = START_SCREEN;

void DrawStartScreen(Rectangle startButton, Rectangle multiButton, Rectangle instructionsButton, Rectangle exitButton) {
    // Desenho de Fundo e Título
    ClearBackground(RAYWHITE);
    DrawText("RAYLIB TETRIS", SCREEN_WIDTH / 2 - MeasureText("RAYLIB TETRIS", 50) / 2, 100, 50, DARKBLUE);
    
    DrawRectangleRec(startButton, LIGHTGRAY);
    DrawRectangleLinesEx(startButton, 2, DARKGRAY);
    DrawText("START GAME", startButton.x + startButton.width / 2 - MeasureText("START GAME", 20) / 2, startButton.y + startButton.height / 2 - 10, 20, BLACK);

    DrawRectangleRec(instructionsButton, LIGHTGRAY);
    DrawRectangleLinesEx(instructionsButton, 2, DARKGRAY);
    DrawText("INSTRUÇÕES", instructionsButton.x + instructionsButton.width / 2 - MeasureText("INSTRUÇÕES", 20) / 2, instructionsButton.y + instructionsButton.height / 2 - 10, 20, BLACK);

    DrawRectangleRec(multiButton, LIGHTGRAY);
    DrawRectangleLinesEx(multiButton, 2, DARKGRAY);
    DrawText("MULTIPLAYER", multiButton.x + multiButton.width/2 - MeasureText("MULTIPLAYER", 20)/2, multiButton.y + 15, 20, BLACK);

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

    std::string serverAddress = "127.0.0.1";
    int port = 7788;
    Texture2D background = LoadTexture("assets/background.jpeg");

    Game game(NORMAL_SPEED, FAST_SPEED, background);
    MatchManager match(NORMAL_SPEED, FAST_SPEED, background, serverAddress, port); //TODO: Create multiGame just if the button is pressed;

    Rectangle startButton = {SCREEN_WIDTH / 2 - 100, 220, 200, 50};
    Rectangle multiButton = {SCREEN_WIDTH / 2 - 100, 290, 200, 50};
    Rectangle instructionsButton = {SCREEN_WIDTH / 2 - 100, 360, 200, 50};
    Rectangle exitButton = {SCREEN_WIDTH / 2 - 100, 430, 200, 50};

    while (!WindowShouldClose() && gameState != EXITING) {
        
        if (gameState == START_SCREEN) {
            Vector2 mousePoint = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, startButton)) {
                    gameState = PLAYING;
                } else if (CheckCollisionPointRec(mousePoint, multiButton)) {
                    multiGame.initNetwork();
                    multiGame.connectToServer();
                    gameState = PLAYING_MULTIPLAYER;
                } else if (CheckCollisionPointRec(mousePoint, instructionsButton)) {
                    gameState = INSTRUCTIONS; 
                } else if (CheckCollisionPointRec(mousePoint, exitButton)) {
                    gameState = EXITING; 
                }
            }
        } 
        if (gameState == PLAYING && game.getGameOver()) gameState = EXITING;
        if (gameState == PLAYING_MULTIPLAYER && multiGame.getGameOver()) gameState = EXITING;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (gameState) {
            case PLAYING:
                game.run();
                game.draw();
                break;

            case PLAYING_MULTIPLAYER:
                multiGame.run();
                multiGame.draw(); 
                break;

            case INSTRUCTIONS:
            case START_SCREEN:
                DrawStartScreen(startButton, multiButton, instructionsButton, exitButton);
                if (gameState == INSTRUCTIONS) {
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(RAYWHITE, 0.9f));
                    DrawText("INSTRUÇÕES...", 50, 100, 30, DARKGRAY);
                    if (IsKeyPressed(KEY_ENTER)) gameState = START_SCREEN;
                }
                break;
        }

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}