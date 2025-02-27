#pragma once

namespace RaySnake 
{
    enum class GameMode
    {
        MainMenu = 0,
        Playing = 1,
        Options = 2,
        EndScreen = 3,
        Exit = 4
    };

    struct AppSettings
    {
        AppSettings() :
            IsRunning(false), FrameTime(0.0f), GameMode(RaySnake::GameMode::MainMenu), AppScaling(2.0f) {};

        // game state and stats
        bool IsRunning;
        float FrameTime;
        RaySnake::GameMode GameMode;
        float AppScaling;
    };
}
