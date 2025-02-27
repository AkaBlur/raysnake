#include <cmath>
#include <iostream>

#include "raylib.h"

#include "RaySnake.h"

#include "Layer/GameLayer.h"
#include "Layer/MenuLayer.h"
#include "Util/Timer.h"


struct AppLayers
{
    AppLayers(RaySnake::AppSettings& settings) :
        Menu{settings}, SnakeGame(settings) {};

    MenuLayer Menu;
    GameLayer SnakeGame;
};

void MenuLoop(RaySnake::AppSettings& settings, AppLayers& layerStack)
{
    layerStack.Menu.OnUpdate(settings.FrameTime);
    layerStack.Menu.Draw(settings.FrameTime);
}

void GameLoop(RaySnake::AppSettings& settings, AppLayers& layerStack)
{
    layerStack.SnakeGame.OnUpdate(settings.FrameTime);
    layerStack.SnakeGame.Draw();
}

void EndScreen(RaySnake::AppSettings& settings, float screenTime, float totalFadeTime)
{
    float FadeRatio = 1.0f - (screenTime / totalFadeTime);
    Vector2 ScreenMid = {(float) GetScreenWidth() * 0.5f, (float) GetScreenHeight() * 0.5f};

    float AlphaValue = 255;
    AlphaValue *= FadeRatio;
    Color TextColor = {0, 0, 0, (unsigned char) round(AlphaValue)};

    DrawText("Thanks for playing!", ScreenMid.x - 170.0f * settings.AppScaling, ScreenMid.y - 50.f * settings.AppScaling, 36 * settings.AppScaling, TextColor);

}

int main(int argc, char* argv[])
{
    RaySnake::AppSettings Settings;

    InitWindow(640 * Settings.AppScaling, 480 * Settings.AppScaling, "Totally accurate Snake-Settings");
    SetTargetFPS(288);

    AppLayers LayerStack(Settings);

    Settings.IsRunning = true;

    Timer EndScreenTimer;
    Settings.GameMode = RaySnake::GameMode::MainMenu;

    while (!WindowShouldClose() && Settings.IsRunning)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Settings.FrameTime = GetFrameTime();

        // FPS counter
        char FPS[10] = "";
        sprintf(FPS, "%d", GetFPS());
        DrawText(FPS, 2, 2, 12, GREEN);

        switch (Settings.GameMode)
        {
            case RaySnake::GameMode::MainMenu:
                MenuLoop(Settings, LayerStack);
                break;

            case RaySnake::GameMode::Playing:
                GameLoop(Settings, LayerStack);
                break;

            case RaySnake::GameMode::Options:
                break;

            case RaySnake::GameMode::EndScreen:
            {
                float TimerVal = EndScreenTimer.Tick(Settings.FrameTime);
                EndScreen(Settings, TimerVal, 2.0f);
                if (TimerVal > 2.0f)
                    Settings.GameMode = RaySnake::GameMode::Exit;

                break;
            }

            case RaySnake::GameMode::Exit:
                Settings.IsRunning = false;
                break;

            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}