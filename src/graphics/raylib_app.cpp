// raylib_app.cpp
#include "raylib_app.hpp"

// Keep speeds for restart (no header changes)
static int sNormalSpeed = 0;
static int sFastSpeed   = 0;
static std::string sServerAddress = "";
static int sPort = 0;

RaylibApp::RaylibApp(int screenW, int screenH, int normalSpeed, int fastSpeed, std::string serverAddress, int port)
    : screenWidth(screenW),
      screenHeight(screenH),
      state(AppState::StartScreen),
      background({0}),
      uiFont({0}),
      startButton({0}),
      multiplayerButton({0}),
      instructionsButton({0}),
      exitButton({0}),
      startTime(0.0f),
      framesCounter(0),
      letterCount(0),
      name("\0"),
      game(normalSpeed, fastSpeed, 500, 65),
      match(std::make_unique<MatchManager>(normalSpeed, fastSpeed, serverAddress, port))
{
    // Store speeds for restart
    sNormalSpeed = normalSpeed;
    sFastSpeed   = fastSpeed;
    sServerAddress = serverAddress;
    sPort = port;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetExitKey(KEY_NULL); // Disable default ESC-to-close
    SetTargetFPS(60);

    background = LoadTexture("assets/background.jpeg");

    uiFont = GetFontDefault();

    buildDemoPieces();
}

// Center a rectangle on (cx, cy)
static Rectangle CenterRect(float cx, float cy, float w, float h) {
    return Rectangle{cx - w * 0.5f, cy - h * 0.5f, w, h};
}

// Bounds of local block cells (x=row, y=col)
static void BoundsOfCells(const std::vector<Position>& cells,
                          int& minX, int& maxX,
                          int& minY, int& maxY)
{
    minX =  9999; maxX = -9999;
    minY =  9999; maxY = -9999;

    for (const auto& p : cells) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }
}

// Draws a block centered inside a panel (NEXT preview)
static void DrawBlockPreview(const Block& b, Rectangle panel, Color fill, int cellSize) {
    auto cells = b.getBlocks();

    int minX, maxX, minY, maxY;
    BoundsOfCells(cells, minX, maxX, minY, maxY);

    float wCells = (float)(maxY - minY + 1);
    float hCells = (float)(maxX - minX + 1);

    float drawW = wCells * (float)cellSize;
    float drawH = hCells * (float)cellSize;

    float startX = panel.x + panel.width * 0.5f - drawW * 0.5f;
    float startY = panel.y + panel.height * 0.5f - drawH * 0.5f;

    for (const auto& c : cells) {
        float px = startX + (float)(c.y - minY) * (float)cellSize;
        float py = startY + (float)(c.x - minX) * (float)cellSize;

        Rectangle r{px, py, (float)cellSize, (float)cellSize};

        DrawRectangleRec(r, fill);
        DrawRectangleLinesEx(r, 2.0f, Fade(BLACK, 0.75f));
        DrawRectangleLinesEx(Rectangle{px + 1, py + 1, (float)cellSize - 2.0f, (float)cellSize - 2.0f},
                             1.0f, Fade(RAYWHITE, 0.20f));
    }
}

// Game over overlay
static void DrawGameOverOverlay(int screenW, int screenH, Font font, const Stat& st, bool gameWin) {
    DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.65f));

    Rectangle card = CenterRect(screenW * 0.5f, screenH * 0.5f, 560, 300);
    DrawRectangleRounded(card, 0.10f, 14, Fade(RAYWHITE, 0.95f));
    DrawRectangleRoundedLines(card, 0.10f, 14, Fade(BLACK, 0.25f));

    const char* titleLost = "GAME OVER";
    const char* titleWin = "GAME WIN";

    Vector2 tSize = MeasureTextEx(font, gameWin ? titleWin : titleLost, 52.0f, 2.0f);
    DrawTextEx(font, gameWin ? titleWin : titleLost,
               {card.x + card.width * 0.5f - tSize.x * 0.5f, card.y + 40},
               52.0f, 2.0f, gameWin ? SKYBLUE : MAROON);

    // Show final score
    std::string scoreLine = "Score: " + st.strScore();
    Vector2 sSize = MeasureTextEx(font, scoreLine.c_str(), 24.0f, 1.0f);
    DrawTextEx(font, scoreLine.c_str(),
               {card.x + card.width * 0.5f - sSize.x * 0.5f, card.y + 120},
               24.0f, 1.0f, DARKGRAY);

    const char* msg1 = "ENTER: restart";
    Vector2 m1 = MeasureTextEx(font, msg1, 22.0f, 1.0f);
    DrawTextEx(font, msg1,
               {card.x + card.width * 0.5f - m1.x * 0.5f, card.y + 175},
               22.0f, 1.0f, DARKBLUE);

    const char* msg2 = "M: back to menu";
    Vector2 m2 = MeasureTextEx(font, msg2, 18.0f, 1.0f);
    DrawTextEx(font, msg2,
               {card.x + card.width * 0.5f - m2.x * 0.5f, card.y + 215},
               18.0f, 1.0f, Fade(DARKGRAY, 0.9f));

    const char* msg3 = "ESC: quit";
    Vector2 m3 = MeasureTextEx(font, msg3, 18.0f, 1.0f);
    DrawTextEx(font, msg3,
               {card.x + card.width * 0.5f - m3.x * 0.5f, card.y + 245},
               18.0f, 1.0f, Fade(DARKGRAY, 0.9f));
}


RaylibApp::~RaylibApp() {
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
            else if (CheckCollisionPointRec(mouse, multiplayerButton)) {
                state = AppState::InputUsername;
                letterCount = 0;
                name[0] = '\0';
            }
            else if (CheckCollisionPointRec(mouse, instructionsButton)) state = AppState::Instructions;
            else if (CheckCollisionPointRec(mouse, exitButton)) state = AppState::Exiting;
        }

        if (IsKeyPressed(KEY_ENTER))  state = AppState::Playing;
        if (IsKeyPressed(KEY_I))      state = AppState::Instructions;
        if (IsKeyPressed(KEY_ESCAPE)) state = AppState::Exiting;
    }
    else if (state == AppState::InputUsername) {
        // Typing Username
        int key = GetCharPressed();

        // A lot of characters in a frame
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)) {
                name[letterCount] = (char)key;
                name[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }

        // Cancel e return to menu
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = AppState::StartScreen;
        }

        // Confirm and connect
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
            try {
                // Connects to web
                match->init(); 
                match->setLocalPlayerName(std::string(name));
                
                // Go to multiplayer
                state = AppState::Multiplayer; 
            }
            catch (const ServerConnectionException& e) {
                std::cerr << "[Attention] " << e.what() << "\n";
                state = AppState::StartScreen; 
            }
            catch (const EnetException& e) {
                std::cerr << "[NetworkError] " << e.what() << "\n";
                state = AppState::Exiting;
            }
            catch (const std::exception& e) {
                std::cerr << "[CRASH] " << e.what() << "\n";
                state = AppState::Exiting;
            }
        }
    }
    else if (state == AppState::Instructions) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
            state = AppState::StartScreen;
        }
    }
    else if(state == AppState::Multiplayer){
        if(IsKeyPressed(KEY_ENTER)) match->readyToStart();
        if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_BACKSPACE)) {
            state = AppState::StartScreen;
            match = std::make_unique<MatchManager>(sNormalSpeed, sFastSpeed, sServerAddress, sPort);
            std::cout << "Returning\n";
        }
    }  
    else if (state == AppState::Playing) {
        // Restart when game is over
        if (game.getGameOver() && IsKeyPressed(KEY_ENTER)) {
            game = Game(sNormalSpeed, sFastSpeed);
        }

        // Back to menu (does not close the window)
        if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_BACKSPACE)) {
            state = AppState::StartScreen;
            game = Game(sNormalSpeed, sFastSpeed,500, 65);
        }

        // Quit game
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = AppState::Exiting;
        }
    }

    // Gameplay input is handled inside Game
}

void RaylibApp::update() {
    // Keep the menu animation running
    if (state == AppState::StartScreen) {
        startTime += GetFrameTime();
    }
    if (state == AppState::InputUsername) {
        framesCounter++;
    }
    if (state == AppState::Playing) {
        // Freeze logic when game over
        if (!game.getGameOver()) {
            game.run();
        }
    }
    else if (state == AppState::Multiplayer) {
        match->run(); 
    }
}

void RaylibApp::draw() {
    if (state == AppState::Playing) {
        drawGameplayBackground();
        drawGameplay(game.getGrid(), game.getCurrentBlock(), game.getStats(), game.getProjectionLine(), game.getGameOver(), false);
    }
    else if (state == AppState::Multiplayer) {
        drawGameplayBackground();

        // Draw remote player
        if (match->getPlayer2() != nullptr) {
            drawGameplay(match->getPlayer2()->getGrid(), 
                        match->getPlayer2()->getCurrentBlock(), 
                        match->getPlayer2()->getStats(), 
                        -1,
                        false,
                        false,
                        match->getPlayer2()->getUsername());
        }
        //Draw local player
        drawGameplay(match->getPlayer1().getGrid(), 
                    match->getPlayer1().getCurrentBlock(), 
                    match->getPlayer1().getStats(), 
                    match->getPlayer1().getProjectionLine(),
                    match->getEnd(),
                    match->getWin(),
                    match->getPlayer1().getUsername());
    }
    else if (state == AppState::InputUsername) drawInputUsername();
    else drawStartScreen();
}

void RaylibApp::drawGameplayBackground(){
    ClearBackground(RAYWHITE);

    // Background tiling
    DrawTextureEx(background, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background, (Vector2){(float)background.width, 0.0f}, 0.0f, 1.0f, WHITE);
}

void RaylibApp::drawGameplay(const Grid& g, const Block& b, const Stat& st, int projRow, bool gameOver, bool win, std::string name) {
    // Draw the grid
    drawGrid(g);

    const auto palette = Colors::getColors();
    Color fill = palette[b.getId()];

    // Ghost first (behind the piece)
    if (projRow != -1) {
        drawBlockCells(b, g, projRow, b.getCol(), fill, true);
    }

    // Current block
    drawBlockCells(b, g, b.getRow(), b.getCol(), fill, false);

    // Draw ghost piece
    if (projRow != -1) {
        drawBlockCells(b, g, projRow, b.getCol(), fill, true);
    }

    // Draw Hud
    drawHUD(g.getPosition(), st, name);

    //Draw GameOver Screen
    if (gameOver) {
        DrawGameOverOverlay(screenWidth, screenHeight, uiFont, st, win);
    }
}

void RaylibApp::drawInputUsername() {
    ClearBackground(BLACK);

    // Draw background (consistent with the Start Screen)
    DrawTextureEx(background, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background, (Vector2){(float)background.width, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));

    // Central Panel
    float panelW = 500.0f;
    float panelH = 250.0f;
    Rectangle panel = CenterRect(screenWidth * 0.5f, screenHeight * 0.5f, panelW, panelH);

    DrawRectangleRounded(panel, 0.1f, 10, Fade(RAYWHITE, 0.95f));
    DrawRectangleRoundedLines(panel, 0.1f, 10, DARKBLUE);

    // Title
    const char* text = "ENTER YOUR USERNAME";
    Vector2 textSize = MeasureTextEx(uiFont, text, 30, 2);
    DrawTextEx(uiFont, text, 
               (Vector2){panel.x + panelW/2 - textSize.x/2, panel.y + 30}, 
               30, 2, DARKBLUE);

    // Input Box (Text Field)
    Rectangle inputBox = {panel.x + 50, panel.y + 100, panelW - 100, 50};
    DrawRectangleRec(inputBox, LIGHTGRAY);
    DrawRectangleLinesEx(inputBox, 2, DARKGRAY);

    // Render the typed username
    Vector2 nameSize = MeasureTextEx(uiFont, name, 40, 2);
    // Align text to the left inside the box with padding
    DrawTextEx(uiFont, name, (Vector2){inputBox.x + 10, inputBox.y + 5}, 40, 2, MAROON);

    // Blinking Cursor (Pipe |)
    if (letterCount < MAX_INPUT_CHARS) {
        // Blink every 30 frames (approx. 0.5 seconds at 60 FPS)
        if (((framesCounter / 30) % 2) == 0) {
            DrawText("|", (int)inputBox.x + 10 + (int)nameSize.x, (int)inputBox.y + 5, 40, MAROON);
        }
    }

    // Footer Instructions (Subtitle)
    const char* sub = "Press ENTER to Connect or ESC to Cancel";
    Vector2 subSize = MeasureTextEx(uiFont, sub, 20, 1);
    DrawTextEx(uiFont, sub, 
               (Vector2){panel.x + panelW/2 - subSize.x/2, panel.y + 180}, 
               20, 1, DARKGRAY);
}

void RaylibApp::drawHUD(Vector2 gridPos, const Stat& st, std::string name) {
    float hudX = gridPos.x + 320; 

    DrawText("SCORE", hudX, 50, 30, WHITE);
    DrawRectangle(hudX - 55, 75, 200, 90, Fade(BLACK, 0.35f));
    DrawText(st.strScore().c_str(), hudX + 35, 90, 60, WHITE);
    DrawText(st.strScore().c_str(), hudX + 35, 90, 60, WHITE);

    DrawRectangle(hudX - 315, 15, MeasureText(name.c_str(), 30)+10, 40, Fade(BLACK, 0.35f));
    DrawText(name.c_str(), hudX - 310, 20, 30, WHITE);

    // Next block box
    DrawText("NEXT", hudX, 190, 30, WHITE);
    Rectangle nextPanel = Rectangle{hudX-55, 220, 200, 150};
    DrawRectangleRec(nextPanel, Fade(BLACK, 0.35f));
    DrawRectangleLinesEx(nextPanel, 2.0f, Fade(BLACK, 0.55f));

    const Block& next = game.getNextBlock();
    Color nextFill = Colors::getColors()[next.getId()];
    DrawBlockPreview(next, nextPanel, nextFill, 20);

    DrawText("LEVEL", hudX, 550, 30, WHITE);
    DrawRectangle(hudX-55, 585, 200, 90, Fade(BLACK, 0.35f));
    DrawText(st.strLevel().c_str(), hudX + 35, 590, 60, WHITE);
}

// Draw block cells using grid coords
void RaylibApp::drawBlockCells(const Block& b,
                               const Grid& g,
                               int anchorRow,
                               int anchorCol,
                               Color fill,
                               bool ghost)
{
    auto cells = b.getBlocks();
    Vector2 off = g.getPosition();
    int size = g.getSize();

    for (const auto& c : cells) {
        float px = off.x + (float)size * (float)(anchorCol + c.y);
        float py = off.y + (float)size * (float)(anchorRow + c.x);

        Rectangle r{px, py, (float)size, (float)size};

        if (ghost) {
            // Landing projection
            DrawRectangleRec(r, Fade(fill, 0.28f));
            DrawRectangleLinesEx(r, 2.0f, Fade(BLACK, 0.50f));
            DrawRectangleLinesEx(Rectangle{px + 1, py + 1, (float)size - 2.0f, (float)size - 2.0f},
                                 1.0f, Fade(RAYWHITE, 0.12f));

            // Small inner highlight
            DrawRectangleRec(Rectangle{px + 3, py + 3, (float)size - 6.0f, (float)size - 6.0f},
                             Fade(RAYWHITE, 0.05f));
        } else {
            // Normal block
            DrawRectangleRec(r, fill);
            DrawRectangleLinesEx(r, 2.0f, Fade(BLACK, 0.75f));
            DrawRectangleLinesEx(Rectangle{px + 1, py + 1, (float)size - 2.0f, (float)size - 2.0f},
                                 1.0f, Fade(RAYWHITE, 0.20f));
        }
    }
}

void RaylibApp::drawGrid(const Grid& g){
    const auto palette = Colors::getColors();
    for (int i = 0; i < g.getGridHeight(); i++) {
        for (int j = 0; j < g.getGridWidth(); j++) {
            float position_x = g.getPosition().x;
            float position_y = g.getPosition().y;
            int size = g.getSize();
            DrawRectangle(position_x + size * j, position_y + size * i, size, size, palette[g.getColorXY(i,j)]);
            DrawRectangleLines(position_x + size * j, position_y + size * i, size, size, palette[1]);
        }
    }
}

// ----------------------
// START SCREEN
// ----------------------

void RaylibApp::drawStartScreen() {
    ClearBackground(BLACK);

    // Background + overlay
    DrawTextureEx(background, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background, (Vector2){(float)background.width, 0.0f}, 0.0f, 1.0f, WHITE);
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.35f));

    // Title
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

    // Demo row
    drawStartAnimation();

    // Panel
    float panelW = 420.0f;
    float panelH = 380.0f; // Aumentei de 320 para 380 (ou 400) para caber 4 botões
    Rectangle panel = CenterRect(screenWidth * 0.5f, screenHeight * 0.70f, panelW, panelH);

    Rectangle shadow = panel;
    shadow.x += 6; shadow.y += 8;
    DrawRectangleRounded(shadow, 0.12f, 18, Fade(BLACK, 0.45f));

    DrawRectangleRounded(panel, 0.12f, 18, Fade(RAYWHITE, 0.92f));
    DrawRectangleRoundedLines(panel, 0.12f, 18, Fade(BLACK, 0.35f));

    // Buttons layout
    float btnW = panelW * 0.72f;
    float btnH = 56.0f;
    float cx = panel.x + panelW * 0.5f;
    float y0 = panel.y + 50.0f;

    startButton        = CenterRect(cx, y0,          btnW, btnH);
    multiplayerButton  = CenterRect(cx, y0 + 75.0f,  btnW, btnH); 
    instructionsButton = CenterRect(cx, y0 + 150.0f, btnW, btnH); 
    exitButton         = CenterRect(cx, y0 + 225.0f, btnW, btnH); 

    auto drawButton = [&](Rectangle r, const char* txt) {
        Vector2 mouse = GetMousePosition();
        bool hover = CheckCollisionPointRec(mouse, r);

        Color base = hover ? Fade(SKYBLUE, 0.35f) : Fade(BLACK, 0.05f);
        Color border = hover ? Fade(DARKBLUE, 0.55f) : Fade(BLACK, 0.25f);

        DrawRectangleRounded(r, 0.18f, 16, base);
        DrawRectangleRoundedLines(r, 0.18f, 16, border);

        Vector2 s = MeasureTextEx(uiFont, txt, 28.0f, 2.0f);
        DrawTextEx(uiFont, txt,
                   {r.x + r.width * 0.5f - s.x * 0.5f,
                    r.y + r.height * 0.5f - s.y * 0.5f},
                   28.0f, 2.0f, DARKBLUE);
    };

    drawButton(startButton, "SINGLE PLAYER");
    drawButton(multiplayerButton, "MULTIPLAYER");
    drawButton(instructionsButton, "INSTRUCTIONS");
    drawButton(exitButton, "QUIT");

    if (state == AppState::Instructions) {
        drawInstructionsOverlay();
    }
}

void RaylibApp::drawInstructionsOverlay() {
    // Dark overlay
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
    DrawText("ENTER: back to menu", x, y, 22, DARKGRAY); y += 55;

    DrawText("Bonus: you may get some 5-block pieces!", x, y, 20, Fade(DARKBLUE, 0.8f));
}

// ----------------------
// DEMO ANIMATION
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

void RaylibApp::drawStartAnimation() {
    const auto palette = Colors::getColors();

    float baseY = 220.0f;
    int cell = 18;
    float spacing = 28.0f;

    // Smooth motion (draw with floats)
    float t = startTime;
    float bob = 8.0f * std::sin(t * 2.2f);

    // Compute total width to center the row
    float totalW = 0.0f;
    std::vector<float> pieceW;
    pieceW.reserve(demoPieces.size());

    for (auto& p : demoPieces) {
        auto cells = p->getBlocks();
        int minX, maxX, minY, maxY;
        BoundsOfCells(cells, minX, maxX, minY, maxY);
        float w = (float)(maxY - minY + 1) * (float)cell;
        pieceW.push_back(w);
        totalW += w;
    }
    totalW += spacing * (float)(demoPieces.size() - 1);

    float x = (float)screenWidth * 0.5f - totalW * 0.5f;

    for (size_t i = 0; i < demoPieces.size(); i++) {
        const Block& p = *demoPieces[i];
        Color fill = palette[p.getId()];

        // Small per-piece offsets
        float localBob = bob + 4.0f * std::sin(t * 2.2f + (float)i * 0.6f);
        float drift    = 2.0f * std::sin(t * 1.6f + (float)i * 0.8f);

        auto cells = p.getBlocks();
        int minX, maxX, minY, maxY;
        BoundsOfCells(cells, minX, maxX, minY, maxY);

        float boxW = pieceW[i];
        float originX = x + (boxW * 0.5f) + drift;
        float originY = baseY + localBob;

        for (const auto& c : cells) {
            float px = originX + (c.y - (minY + maxY) * 0.5f) * (float)cell;
            float py = originY + (c.x - (minX + maxX) * 0.5f) * (float)cell;

            Rectangle r{px, py, (float)cell, (float)cell};

            DrawRectangleRec(r, fill);
            DrawRectangleLinesEx(r, 2.0f, Fade(BLACK, 0.80f));
            DrawRectangleLinesEx(Rectangle{px + 1, py + 1, (float)cell - 2.0f, (float)cell - 2.0f},
                                 1.0f, Fade(RAYWHITE, 0.18f));
        }

        x += boxW + spacing;
    }
}
