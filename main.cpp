float screenWidth = 1280;
float screenHeight = 720;
float zoomFactorX;
float zoomFactorY;
#include <raylib.h>
#include "DisplayManager.hpp"

int main(){
    const int turnSpeed = 350;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Five Guys in Freddy");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(0));
    Image icon = LoadImage("res/tex/freddyIcon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    Camera2D camera = { 0 };
    camera.target = Vector2{(float)screenWidth/2, 0};
    camera.offset = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    //Draw loading text
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Loading...", screenWidth/2-MeasureText("Loading...", 40)/2, screenHeight/2-20, 40, WHITE);
    EndDrawing();

    DisplayManager manager(turnSpeed, &camera); //Creating this loads all the textures and sounds

    SetMousePosition(screenWidth/2, screenHeight/2);
    while (!WindowShouldClose()) {
        screenWidth = (float)GetScreenWidth();
        int aspectFactor = screenWidth/16;
        screenHeight = aspectFactor*9;

        // Calculate zoom factor
        float zoomFactorX = screenWidth / 1280;
        float zoomFactorY = screenHeight / 720;
        camera.zoom = (zoomFactorX < zoomFactorY) ? zoomFactorX : zoomFactorY;

        SetWindowTitle(TextFormat("Gang bang at freddys FPS:%i", GetFPS()));
        manager.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        manager.Draw();
        // Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), camera);
        // DrawText(TextFormat("%i, %i", (int)mPos.x, (int)mPos.y), 0, 0, 24, WHITE);
        // DrawText(TextFormat("Zoom:%f", camera.zoom), 0, 24, 24, WHITE);
        // DrawText(TextFormat("%f", camera.target.x), 0, 44, 24, WHITE);
        // DrawText(TextFormat("%f", (float)GetScreenWidth()), 0, 70, 24, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}