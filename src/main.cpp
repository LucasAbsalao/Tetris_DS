#include <raylib.h>
#include<iostream>
#include<string>
#include "game.hpp"
#include "matchManager.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NORMAL_SPEED 15
#define FAST_SPEED 2
#define MAX_INPUT_CHARS 12

enum GameState {
    START_SCREEN,
    INPUT_USERNAME,
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

    // --- VARIÁVEIS PARA O INPUT DE TEXTO ---
    char name[MAX_INPUT_CHARS + 1] = "\0"; // Buffer para o texto (+1 para o terminador nulo)
    int letterCount = 0;
    Rectangle textBox = { SCREEN_WIDTH / 2.0f - 150, SCREEN_HEIGHT / 2.0f, 300, 50 };
    int framesCounter = 0; // Para piscar o cursor
    // ---------------------------------------

    Rectangle startButton = {SCREEN_WIDTH / 2 - 100, 220, 200, 50};
    Rectangle multiButton = {SCREEN_WIDTH / 2 - 100, 290, 200, 50};
    Rectangle instructionsButton = {SCREEN_WIDTH / 2 - 100, 360, 200, 50};
    Rectangle exitButton = {SCREEN_WIDTH / 2 - 100, 430, 200, 50};

    while (!WindowShouldClose() && gameState != EXITING) {
        
        switch(gameState) {
            case START_SCREEN: {
                Vector2 mousePoint = GetMousePosition();
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePoint, startButton)) {
                        gameState = PLAYING;
                    } else if (CheckCollisionPointRec(mousePoint, multiButton)) {
                        gameState = INPUT_USERNAME; // Vai para a tela de nome
                    } else if (CheckCollisionPointRec(mousePoint, instructionsButton)) {
                        gameState = INSTRUCTIONS; 
                    } else if (CheckCollisionPointRec(mousePoint, exitButton)) {
                        gameState = EXITING; 
                    }
                }
                break;
            }

            case INPUT_USERNAME: {
                int key = GetCharPressed();

                while (key > 0) {
                    if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)) {
                        name[letterCount] = (char)key;
                        name[letterCount+1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();  // Pega o próximo da fila
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    letterCount--;
                    if (letterCount < 0) letterCount = 0;
                    name[letterCount] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                    try{
                        match.init();
                        match.setLocalPlayerName(std::string(name));
                        gameState = PLAYING_MULTIPLAYER;
                    }
                    catch (const ServerConnectionException& e) {
                        std::cerr << "[Attention] " << e.what() << "\n";
                        gameState = START_SCREEN;
                    }
                    // Captura erros genéricos da sua rede
                    catch (const EnetException& e) {
                        std::cerr << "[NetworkError] " << e.what() << "\n";
                        return -1; 
                    }
                    // Captura qualquer outra loucura
                    catch (const std::exception& e) {
                        std::cerr << "[CRASH] " << e.what() << "\n";
                        return -1;
                    }
                    
                }
                framesCounter++;
                break;
            }

            case PLAYING:
                if (game.getGameOver()) gameState = EXITING;
                break;
                
            case PLAYING_MULTIPLAYER:
                // if (match.getGameOver()) gameState = EXITING;
                if(IsKeyPressed(KEY_ENTER)) match.readyToStart();
                break;
            
            case INSTRUCTIONS:
                 if (IsKeyPressed(KEY_ENTER)) gameState = START_SCREEN;
                 break;
        }
        //if (gameState == PLAYING_MULTIPLAYER && match.getGameOver()) gameState = EXITING;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (gameState) {
            case PLAYING:
                game.run();
                game.draw();
                break;

            case PLAYING_MULTIPLAYER:
                match.run();
                match.draw(); 
                break;

            case INSTRUCTIONS:
                 DrawStartScreen(startButton, multiButton, instructionsButton, exitButton);
                 DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(RAYWHITE, 0.9f));
                 DrawText("INSTRUÇÕES...", 50, 100, 30, DARKGRAY);
                 DrawText("Pressione ENTER para voltar", 50, 200, 20, DARKGRAY);
                 break;

            case START_SCREEN:
                DrawStartScreen(startButton, multiButton, instructionsButton, exitButton);
                break;

            // --- DESENHO DA TELA DE INPUT ---
            case INPUT_USERNAME:
                DrawText("DIGITE SEU NOME DE USUÁRIO", SCREEN_WIDTH/2 - MeasureText("DIGITE SEU NOME DE USUÁRIO", 20)/2, SCREEN_HEIGHT/2 - 50, 20, GRAY);

                DrawRectangleRec(textBox, LIGHTGRAY);
                DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

                DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
                
                DrawText(TextFormat("Chars: %i/%i", letterCount, MAX_INPUT_CHARS), SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 + 60, 20, DARKGRAY);

                // Cursor piscando
                if (letterCount < MAX_INPUT_CHARS) {
                    if (((framesCounter / 20) % 2) == 0) {
                        DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
                    }
                }
                
                DrawText("Pressione ENTER para conectar", SCREEN_WIDTH/2 - MeasureText("Pressione ENTER para conectar", 20)/2, SCREEN_HEIGHT - 100, 20, GRAY);
                break;
        }

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}