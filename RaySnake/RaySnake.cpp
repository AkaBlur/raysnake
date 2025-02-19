#include <cmath>
#include <iostream>

#include "raylib.h"

#include "RaySnake.h"

#include "Layer/GameLayer.h"
#include "Layer/MenuLayer.h"
#include "Util/Timer.h"


struct LayerStack {
    LayerStack(RaySnake::GameStack& stack) :
        Menu{stack}, SnakeGame(stack) {};

    MenuLayer Menu;
    GameLayer SnakeGame;
};

void MenuLoop(RaySnake::GameStack& gameStack, LayerStack& layerStack) {
    layerStack.Menu.OnUpdate(gameStack.FrameTime);
    layerStack.Menu.Draw(gameStack.FrameTime);
}

void GameLoop(RaySnake::GameStack& gameStack, LayerStack& layerStack) {
    layerStack.SnakeGame.OnUpdate(gameStack.FrameTime);
    layerStack.SnakeGame.Draw();
}

void EndScreen(RaySnake::GameStack& gameStack, float screenTime, float totalFadeTime) {
    float FadeRatio = 1.0f - (screenTime / totalFadeTime);
    Vector2 ScreenMid = {(float) GetScreenWidth() * 0.5f, (float) GetScreenHeight() * 0.5f};

    float AlphaValue = 255;
    AlphaValue *= FadeRatio;
    Color TextColor = {0, 0, 0, (unsigned char) round(AlphaValue)};

    DrawText("Thanks for playing!", ScreenMid.x - 170.0f * gameStack.GameScaling, ScreenMid.y - 50.f * gameStack.GameScaling, 36 * gameStack.GameScaling, TextColor);

}

int main(int argc, char* argv[])
{
    RaySnake::GameStack Game;

    InitWindow(640 * Game.GameScaling, 480 * Game.GameScaling, "Totally accurate Snake-Game");
    SetTargetFPS(288);

    LayerStack GameLayers(Game);

    Game.IsRunning = true;

    Timer EndScreenTimer;
    Game.GameMode = RaySnake::GameMode::MainMenu;

    while (!WindowShouldClose() && Game.IsRunning)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Game.FrameTime = GetFrameTime();

        // FPS counter
        char FPS[10] = "";
        sprintf(FPS, "%d", GetFPS());
        DrawText(FPS, 2, 2, 12, GREEN);

        switch (Game.GameMode) {
            case RaySnake::GameMode::MainMenu:
                MenuLoop(Game, GameLayers);
                break;

            case RaySnake::GameMode::Playing:
                GameLoop(Game, GameLayers);
                break;

            case RaySnake::GameMode::Options:
                break;

            case RaySnake::GameMode::EndScreen: {
                float TimerVal = EndScreenTimer.Tick(Game.FrameTime);
                EndScreen(Game, TimerVal, 2.0f);
                if (TimerVal > 2.0f)
                    Game.GameMode = RaySnake::GameMode::Exit;

                break;
            }

            case RaySnake::GameMode::Exit:
                Game.IsRunning = false;
                break;

            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}