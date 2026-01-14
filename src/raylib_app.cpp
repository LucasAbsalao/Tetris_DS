#include "raylib_app.hpp"
#include "colors.hpp"

// Creates the application and initializes Raylib
RaylibApp::RaylibApp(int screenW, int screenH, int normalSpeed, int fastSpeed)
    : screenWidth(screenW),
      screenHeight(screenH),
      state(AppState::StartScreen),
      background({0}),
      startButton({screenW / 2.0f - 100, 300, 200, 50}),
      instructionsButton({screenW / 2.0f - 100, 370, 200, 50}),
      exitButton({screenW / 2.0f - 100, 440, 200, 50}),
      game(normalSpeed, fastSpeed) // Game contains only logic
{
    // Initialize window and frame rate
    InitWindow(screenWidth, screenHeight, "raylib Tetris");
    SetTargetFPS(60);

    // Load graphical resources
    background = LoadTexture("assets/background.jpeg");
}

// Releases Raylib resources
RaylibApp::~RaylibApp() {
    UnloadTexture(background);
    CloseWindow();
}

// Main application loop
void RaylibApp::run() {
    while (!WindowShouldClose() && state != AppState::Exiting) {
        processInput(); // handle user input
        update();       // update game logic

        BeginDrawing();
        draw();         // render current screen
        EndDrawing();
    }
}

// Handles input for menus and instructions
void RaylibApp::processInput() {
    if (state == AppState::StartScreen) {
        Vector2 mousePoint = GetMousePosition();
        bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        // Check which button was clicked
        if (mouseClicked) {
            if (CheckCollisionPointRec(mousePoint, startButton)) {
                state = AppState::Playing;
            } else if (CheckCollisionPointRec(mousePoint, instructionsButton)) {
                state = AppState::Instructions;
            } else if (CheckCollisionPointRec(mousePoint, exitButton)) {
                state = AppState::Exiting;
            }
        }
    }
    else if (state == AppState::Instructions) {
        // Return to menu
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
            state = AppState::StartScreen;
        }
    }

    // Gameplay input is handled inside Game
}

// Updates the game when playing
void RaylibApp::update() {
    if (state == AppState::Playing) {
        game.run();
    }
}

// Chooses what to draw based on the current state
void RaylibApp::draw() {
    if (state == AppState::Playing) {
        drawGameplay();
    } else {
        drawStartScreen();
    }
}

// Draws the gameplay screen
void RaylibApp::drawGameplay() {
    ClearBackground(RAYWHITE);

    // Draw scrolling background
    DrawTextureEx(background, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background, (Vector2){(float)background.width, 0.0f}, 0.0f, 1.0f, WHITE);

    // Access game state
    const Grid& g = game.getGrid();
    const Block& b = game.getCurrentBlock();

    // Map color ID to Raylib color
    static const vector<Color> COLORS = Colors::getColors();
    Color c = COLORS[b.getColorId()];

    // Draw board and active block
    g.draw();
    b.draw(g.getPosition(), c);

    // Draw ghost projection
    int proj = game.getProjectionLine();
    if (proj != -1) {
        b.drawProjection(g.getPosition(), proj, c);
    }

    drawHUD();
}

// Draws score and level information
void RaylibApp::drawHUD() {
    const Stat& st = game.getStats();

    DrawText("TETRIS", 145, 10, 50, WHITE);

    DrawText("SCORE", 455, 50, 30, WHITE);
    DrawRectangle(400, 75, 200, 90, Fade(BLACK, 0.4f));
    DrawText(st.strScore().c_str(), 490, 90, 60, WHITE);

    DrawText("LEVEL", 455, 550, 30, WHITE);
    DrawRectangle(400, 575, 200, 90, Fade(BLACK, 0.4f));
    DrawText(st.strLevel().c_str(), 490, 590, 60, WHITE);
}

// Draws the start menu
void RaylibApp::drawStartScreen() {
    ClearBackground(RAYWHITE);

    DrawText("RAYLIB TETRIS",
             screenWidth / 2 - MeasureText("RAYLIB TETRIS", 50) / 2,
             100, 50, DARKBLUE);

    // Start button
    DrawRectangleRec(startButton, LIGHTGRAY);
    DrawRectangleLinesEx(startButton, 2, DARKGRAY);
    DrawText("START GAME",
             startButton.x + startButton.width / 2 - MeasureText("START GAME", 20) / 2,
             startButton.y + startButton.height / 2 - 10, 20, BLACK);

    // Instructions button
    DrawRectangleRec(instructionsButton, LIGHTGRAY);
    DrawRectangleLinesEx(instructionsButton, 2, DARKGRAY);
    DrawText("INSTRUÇÕES",
             instructionsButton.x + instructionsButton.width / 2 - MeasureText("INSTRUÇÕES", 20) / 2,
             instructionsButton.y + instructionsButton.height / 2 - 10, 20, BLACK);

    // Exit button
    DrawRectangleRec(exitButton, LIGHTGRAY);
    DrawRectangleLinesEx(exitButton, 2, DARKGRAY);
    DrawText("SAIR",
             exitButton.x + exitButton.width / 2 - MeasureText("SAIR", 20) / 2,
             exitButton.y + exitButton.height / 2 - 10, 20, BLACK);

    if (state == AppState::Instructions) {
        drawInstructionsOverlay();
    }
}

// Draws instructions overlay on top of the menu
void RaylibApp::drawInstructionsOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(RAYWHITE, 0.9f));
    DrawText("INSTRUÇÕES DO JOGO:", 50, 100, 30, DARKGRAY);
    DrawText("Use the arrow keys to move and rotate.", 50, 150, 20, DARKGRAY);
    DrawText("Press Enter/Esc to return to the menu.", 50, 200, 20, DARKGRAY);
}
