#pragma once

#include <raylib.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>


#include "../core/game.hpp"
#include "../core/block.hpp"
#include "../core/grid.hpp"
#include "../core/stats.hpp"
#include "../core/tetrominoes.hpp"
#include "../network/matchManager.hpp"
#include "../util/colors.hpp"

// App screens
enum class AppState {
    StartScreen,
    Playing,
    Instructions,
    InputUsername,
    Multiplayer,
    Exiting

};

class RaylibApp {
public:
    RaylibApp(int screenW, int screenH, int normalSpeed, int fastSpeed, std::string serverAddress, int port);
    ~RaylibApp();

    void run();

private:
    void processInput();
    void update();
    void draw();

    // Screens
    void drawStartScreen();
    void drawInstructionsOverlay();
    void drawGameplayBackground();
    void drawGameplay(const Grid& g, const Block& b, const Stat& st, int projRow, bool gameOver, bool win);
    void drawInputUsername();
    void drawMatch();
    void drawHUD(Vector2 gridPos, const Stat& st);

    // Helpers
    void drawBlockCells(const Block& b,
                        const Grid& g,
                        int anchorRow,
                        int anchorCol,
                        Color fill,
                        bool outlineOnly);

    void buildDemoPieces();    // creates one instance of each piece
    void drawStartAnimation(); // animated “piece showcase”
    void drawGrid(const Grid& g); // Draw grid

    //Control variables    
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
    Rectangle multiplayerButton;

    // Start screen animation
    float startTime;
    std::vector<std::unique_ptr<Block>> demoPieces;

    //Text Variables
    char name[16] = {0}; // Max 15 chars + null terminator
    int letterCount;
    int framesCounter;
    const int MAX_INPUT_CHARS = 15;

    // Game logic
    Game game;
    std::unique_ptr<MatchManager> match;
};
