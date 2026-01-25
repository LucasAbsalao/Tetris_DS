// raylib_app.cpp
#include "raylib_app.hpp"
#include "colors.hpp"
#include "tetrominoes.hpp"

#include <algorithm>
#include <cmath>

static Rectangle CenterRect(float cx, float cy, float w, float h) {
    return Rectangle{cx - w * 0.5f, cy - h * 0.5f, w, h};
}

RaylibApp::RaylibApp(int screenW, int screenH, int normalSpeed, int fastSpeed)
    : screenWidth(screenW),
      screenHeight(screenH),
      state(AppState::StartScreen),
      background({0}),
      uiFont({0}),
      startButton({0}),
      instructionsButton({0}),
      exitButton({0}),
      startTime(0.0f),
      game(normalSpeed, fastSpeed)
{
    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    background = LoadTexture("assets/background.jpeg");

    // If you don't have a ttf yet, you can comment this and use default font.
    // uiFont = LoadFontEx("assets/ui.ttf", 96, nullptr, 0);
    uiFont = GetFontDefault();

    buildDemoPieces();
}

RaylibApp::~RaylibApp() {
    // Only unload if you used LoadFontEx. If using default font, unloading is not needed.
    // UnloadFont(uiFont);
    UnloadTexture(background);
    CloseWindow();
}

void RaylibApp::run() {
    while (!WindowShouldClose() && state != AppState::Exiting) {
        processInput();
        update();

        BeginDrawing();
        draw();
        EndDrawing();
    }
}

void RaylibApp::processInput() {
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (state == AppState::StartScreen) {
        if (click) {
            if (CheckCollisionPointRec(mouse, startButton)) state = AppState::Playing;
            else if (CheckCollisionPointRec(mouse, instructionsButton)) state = AppState::Instructions;
            else if (CheckCollisionPointRec(mouse, exitButton)) state = AppState::Exiting;
        }
        if (IsKeyPressed(KEY_ENTER)) state = AppState::Playing;
        if (IsKeyPressed(KEY_I)) state = AppState::Instructions;
        if (IsKeyPressed(KEY_ESCAPE)) state = AppState::Exiting;
    }
    else if (state == AppState::Instructions) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
            state = AppState::StartScreen;
        }
    }
    // Gameplay input stays inside Game::getMovement() (your choice)
}

void RaylibApp::update() {
    // ✅ Fix "travado": animation time advances on StartScreen too
    if (state == AppState::StartScreen) {
        startTime += GetFrameTime();
    }

    if (state == AppState::Playing) {
        game.run();
    }
}

void RaylibApp::draw() {
    if (state == AppState::Playing) drawGameplay();
    else drawStartScreen();
}

void RaylibApp::drawGameplay() {
    ClearBackground(RAYWHITE);

    DrawTextureEx(background, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background, (Vector2){(float)background.width, 0.0f}, 0.0f, 1.0f, WHITE);

    const Grid& g = game.getGrid();
    const Block& b = game.getCurrentBlock();

    g.draw();

    const auto palette = Colors::getColors();
    Color fill = palette[b.getId()];

    // Draw current piece
    drawBlockCells(b, g, b.getRow(), b.getCol(), fill, false);

    // Draw ghost
    int projRow = game.getProjectionLine();
    if (projRow != -1) drawBlockCells(b, g, projRow, b.getCol(), fill, true);

    drawHUD();
}

void RaylibApp::drawHUD() {
    const Stat& st = game.getStats();

    DrawText("TETRIS", 145, 10, 50, WHITE);

    DrawText("SCORE", 455, 50, 30, WHITE);
    DrawRectangle(400, 75, 200, 90, Fade(BLACK, 0.35f));
    DrawText(st.strScore().c_str(), 490, 90, 60, WHITE);

    DrawText("LEVEL", 455, 550, 30, WHITE);
    DrawRectangle(400, 575, 200, 90, Fade(BLACK, 0.35f));
    DrawText(st.strLevel().c_str(), 490, 590, 60, WHITE);
}

// Draw helper: draws 4/5 cells from current rotation + anchor
void RaylibApp::drawBlockCells(const Block& b,
                               const Grid& g,
                               int anchorRow,
                               int anchorCol,
                               Color fill,
                               bool outlineOnly)
{
    auto cells = b.getBlocks();
    Vector2 off = g.getPosition();
    int size = g.getSize();

    for (const auto& c : cells) {
        int row = anchorRow + c.x;
        int col = anchorCol + c.y;

        float px = off.x + size * col;
        float py = off.y + size * row;

        if (!outlineOnly) DrawRectangle(px, py, (float)size, (float)size, fill);
        DrawRectangleLines(px, py, (float)size, (float)size, BLACK);
    }
}

// ----------------------
// START SCREEN (NEW UI)
// ----------------------

void RaylibApp::drawStartScreen() {
    ClearBackground(BLACK);

    // Background with slight dark overlay
    DrawTextureEx(background, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.35f));

    // Title centered
    const char* title = "TETRIS";
    Vector2 titleSize = MeasureTextEx(uiFont, title, 96.0f, 4.0f);
    Vector2 titlePos = {(float)screenWidth * 0.5f - titleSize.x * 0.5f, 70.0f};
    DrawTextEx(uiFont, title, titlePos, 96.0f, 4.0f, RAYWHITE);

    // Subtitle
    const char* sub = "Press ENTER to play";
    Vector2 subSize = MeasureTextEx(uiFont, sub, 22.0f, 1.0f);
    DrawTextEx(uiFont, sub,
               {(float)screenWidth * 0.5f - subSize.x * 0.5f, titlePos.y + 110.0f},
               22.0f, 1.0f, Fade(RAYWHITE, 0.85f));

    // Demo animation centered
    drawStartAnimation();

    // Central panel
    float panelW = 420.0f;
    float panelH = 320.0f;
    Rectangle panel = CenterRect(screenWidth * 0.5f, screenHeight * 0.70f, panelW, panelH);

    // Shadow
    Rectangle shadow = panel;
    shadow.x += 6; shadow.y += 8;
    DrawRectangleRounded(shadow, 0.12f, 18, Fade(BLACK, 0.45f));

    // Panel
    DrawRectangleRounded(panel, 0.12f, 18, Fade(RAYWHITE, 0.92f));
    DrawRectangleRoundedLines(panel, 0.12f, 18, Fade(BLACK, 0.35f));

    // Buttons aligned inside panel
    float btnW = panelW * 0.72f;
    float btnH = 56.0f;
    float cx = panel.x + panelW * 0.5f;
    float y0 = panel.y + 70.0f;

    startButton        = CenterRect(cx, y0,           btnW, btnH);
    instructionsButton = CenterRect(cx, y0 + 80.0f,   btnW, btnH);
    exitButton         = CenterRect(cx, y0 + 160.0f,  btnW, btnH);

    auto drawButton = [&](Rectangle r, const char* txt) {
        Vector2 mouse = GetMousePosition();
        bool hover = CheckCollisionPointRec(mouse, r);

        Color base = hover ? Fade(SKYBLUE, 0.35f) : Fade(BLACK, 0.05f);
        Color border = hover ? Fade(DARKBLUE, 0.55f) : Fade(BLACK, 0.25f);

        DrawRectangleRounded(r, 0.18f, 16, base);
        DrawRectangleRoundedLines(r, 0.18f, 16, border);

        Vector2 s = MeasureTextEx(uiFont, txt, 28.0f, 2.0f);
        DrawTextEx(uiFont, txt,
                   {r.x + r.width * 0.5f - s.x * 0.5f, r.y + r.height * 0.5f - s.y * 0.5f},
                   28.0f, 2.0f, DARKBLUE);
    };

    drawButton(startButton, "PLAY");
    drawButton(instructionsButton, "INSTRUCTIONS");
    drawButton(exitButton, "QUIT");

    if (state == AppState::Instructions) {
        drawInstructionsOverlay();
    }
}

void RaylibApp::drawInstructionsOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.55f));

    Rectangle card = CenterRect(screenWidth * 0.5f, screenHeight * 0.5f, 640, 360);
    DrawRectangleRounded(card, 0.08f, 14, Fade(RAYWHITE, 0.95f));
    DrawRectangleRoundedLines(card, 0.08f, 14, Fade(BLACK, 0.25f));


    int x = (int)card.x + 40;
    int y = (int)card.y + 35;

    DrawTextEx(uiFont, "INSTRUCTIONS", {(float)x, (float)y}, 42, 2.0f, DARKBLUE);
    y += 70;

    DrawText("ARROWS: move left/right", x, y, 22, DARKGRAY); y += 35;
    DrawText("UP: rotate", x, y, 22, DARKGRAY); y += 35;
    DrawText("HOLD DOWN: soft drop", x, y, 22, DARKGRAY); y += 35;
    DrawText("ENTER or ESC: back to menu", x, y, 22, DARKGRAY); y += 55;

    DrawText("Bonus: you may get some 5-block pieces!", x, y, 20, Fade(DARKBLUE, 0.8f));
}

// ----------------------
// START ANIMATION
// ----------------------

void RaylibApp::buildDemoPieces() {
    demoPieces.clear();
    demoPieces.push_back(std::make_unique<LBlock>());
    demoPieces.push_back(std::make_unique<JBlock>());
    demoPieces.push_back(std::make_unique<IBlock>());
    demoPieces.push_back(std::make_unique<OBlock>());
    demoPieces.push_back(std::make_unique<SBlock>());
    demoPieces.push_back(std::make_unique<TBlock>());
    demoPieces.push_back(std::make_unique<ZBlock>());
    demoPieces.push_back(std::make_unique<PBlock>());
    demoPieces.push_back(std::make_unique<UBlock>());
}

static void BoundsOfCells(const std::vector<Position>& cells, int& minX, int& maxX, int& minY, int& maxY) {
    minX =  9999; maxX = -9999;
    minY =  9999; maxY = -9999;
    for (auto& p : cells) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }
}

void RaylibApp::drawStartAnimation() {
    const auto palette = Colors::getColors();

    // Where the demo row lives (under the title)
    float baseY = 220.0f;
    int cell = 18;
    float spacing = 28.0f;

    // Animation: slight vertical bobbing
    float t = startTime;
    float bob = 6.0f * std::sin(t * 2.2f);

    // Compute total width of all pieces (using their bounding boxes)
    float totalW = 0.0f;
    std::vector<float> pieceW;
    pieceW.reserve(demoPieces.size());

    for (auto& p : demoPieces) {
        auto cells = p->getBlocks();
        int minX, maxX, minY, maxY;
        BoundsOfCells(cells, minX, maxX, minY, maxY);
        float w = (maxY - minY + 1) * cell;
        pieceW.push_back(w);
        totalW += w;
    }
    totalW += spacing * (float)(demoPieces.size() - 1);

    // Center start X
    float x = (float)screenWidth * 0.5f - totalW * 0.5f;

    // Draw each piece centered in its own box
    for (size_t i = 0; i < demoPieces.size(); i++) {
        const Block& p = *demoPieces[i];
        Color fill = palette[p.getId()];

        // A little time offset per piece for nicer motion
        float localBob = bob + 4.0f * std::sin(t * 2.2f + (float)i * 0.6f);

        // Draw cells
        auto cells = p.getBlocks();
        int minX, maxX, minY, maxY;
        BoundsOfCells(cells, minX, maxX, minY, maxY);

        // Center piece inside its own width
        float boxW = pieceW[i];
        float originX = x + (boxW * 0.5f);
        float originY = baseY + localBob;

        // Convert local (x=row, y=col) into pixels
        for (auto& c : cells) {
            float px = originX + (c.y - (minY + maxY) * 0.5f) * cell;
            float py = originY + (c.x - (minX + maxX) * 0.5f) * cell;

            DrawRectangle(px, py, (float)cell, (float)cell, fill);
            DrawRectangleLines(px, py, (float)cell, (float)cell, Fade(BLACK, 0.85f));
        }

        x += boxW + spacing;
    }
}
