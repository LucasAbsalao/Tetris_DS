#pragma once

#include <raylib.h>
#include "game.hpp"

// Application states (menu/game/instructions)
enum class AppState {
    StartScreen,
    Playing,
    Instructions,
    Exiting
};

class RaylibApp {
public:
    RaylibApp(int screenW, int screenH, int normalSpeed, int fastSpeed);
    ~RaylibApp();

    void run();

private:
    void processInput();  // menu/instructions input
    void update();        // updates the game when playing
    void draw();          // draws current screen

    // UI screens
    void drawStartScreen();
    void drawInstructionsOverlay();

    // Gameplay rendering (now handled here, since Game::draw was removed)
    void drawGameplay();
    void drawHUD();

private:
    int screenWidth;
    int screenHeight;

    AppState state;

    // Raylib resources (owned by the app)
    Texture2D background;

    // UI buttons
    Rectangle startButton;
    Rectangle instructionsButton;
    Rectangle exitButton;

    // Core game (logic only)
    Game game;
};





