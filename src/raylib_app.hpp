#pragma once

#include <raylib.h>
#include <vector>
#include <memory>

#include "game.hpp"
#include "block.hpp"
#include "grid.hpp"
#include "stats.hpp"

// App screens
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
    void processInput();
    void update();
    void draw();

    // Screens
    void drawStartScreen();
    void drawInstructionsOverlay();
    void drawGameplay();
    void drawHUD();

    // Helpers
    void drawBlockCells(const Block& b,
                        const Grid& g,
                        int anchorRow,
                        int anchorCol,
                        Color fill,
                        bool outlineOnly);

    void buildDemoPieces();    // creates one instance of each piece
    void drawStartAnimation(); // animated “piece showcase”

private:
    int screenWidth;
    int screenHeight;
    AppState state;

    // Raylib resources
    Texture2D background;
    Font uiFont;

    // UI buttons
    Rectangle startButton;
    Rectangle instructionsButton;
    Rectangle exitButton;

    // Start screen animation
    float startTime;
    std::vector<std::unique_ptr<Block>> demoPieces;

    // Game logic
    Game game;
};
