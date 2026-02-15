#include "raylib.h"
#include "game.h"

int main() {
    const int screenW = 800;
    const int screenH = 600;

    InitWindow(screenW, screenH, "Asteroids - Separated Game Structure");
    SetTargetFPS(60);

    Game game;
    game.Init((float)GetScreenWidth(), (float)GetScreenHeight());

    while (!WindowShouldClose()) {
        float w = (float)GetScreenWidth();
        float h = (float)GetScreenHeight();

        game.UpdateFrame(w, h);
        game.Render(w, h);
    }

    CloseWindow();
    return 0;
}


