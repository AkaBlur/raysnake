#pragma once

#include <cstdint>

#include "MenuChoice.h"
#include "raylib.h"
#include "RaySnake.h"
#include "Objects/Snake.h"
#include "Util/Random.h"
#include "Util/Timer.h"

class GameLayer {
public:
    GameLayer(RaySnake::GameStack& mainGameStack) :
        m_GameStack(mainGameStack), m_GameSettings(), m_Snake(m_GameSettings.BoardSize, m_GameSettings.BoardSize),
        m_PauseMenu("Paused"), m_DeadMenu("You died!")
    {
        Init();
        m_RandomEngine = Random::GetRandomEngine();

        m_PauseMenu.AddChoice("Resume", "Esc");
        m_PauseMenu.AddChoice("Exit", "Q");

        m_DeadMenu.AddChoice("Restart", "R");
        m_DeadMenu.AddChoice("Return to Menu", "Esc");
        m_DeadMenu.AddChoice("Exit", "Q");

    };

public:
    void Draw();
    void OnUpdate(float deltaTime);

    void Init();
    void Reset();

private:
    enum class GameState {
        Playing = 0,
        Paused = 1,
        GameOver = 2
    };

    struct Assets {
        // Snake Assets
        Image SnakeHead;
        Image SnakeHeadEat;
        Image SnakeHeadBlink;
        Image SnakeBody;
        Image SnakeTail;
        Image Food;
        Image Mine;
    } m_ImageAssets;

    struct TexturesSnake {
        Texture2D SnakeHead;
        Texture2D SnakeHeadEat;
        Texture2D SnakeHeadBlink;
        Texture2D SnakeBody;
        Texture2D SnakeTail;
        Texture2D Food;
        Texture2D Mine;
    } m_Textures;

    static constexpr float C_SNAKE_MOVESPEED_BASE = 1.0f;
    static constexpr float C_SNAKE_MOVESPEED_MOD = 0.75f;

    struct GameSettings {
        // snakes' time to move in seconds
        float SnakeMoveTime = C_SNAKE_MOVESPEED_BASE;
        // board (quadratic) size in tiles
        uint8_t BoardSize = 20;
    } m_GameSettings;


private:
    RaySnake::GameStack& m_GameStack;
    GameState m_GameState = GameState::Playing;
    Random::RandomEngine m_RandomEngine;

    int m_LastKeyPressed = 0;

    //----------------------------------------------------
    // Game objects
    //----------------------------------------------------
    // ekanS
    Snake m_Snake;
    // probably an apple
    Vector2 m_FoodPos = {0, 0};
    // eaten food
    uint32_t m_FoodCount = 0;

    //----------------------------------------------------
    // Window related
    //----------------------------------------------------
    // window area in pixels on screen
    Vector2 m_WindowArea = {0, 0};
    // area of the play field in pixels
    Vector2 m_GameArea = {0, 0};
    // starting point of the game field (top right corner)
    Vector2 m_GameAreaTopCorner = {0, 0};
    // pause menu
    MenuChoice m_PauseMenu;
    // dead menu
    MenuChoice m_DeadMenu;

    //----------------------------------------------------
    // Timings
    //----------------------------------------------------
    // timer for snakes' advancing
    Timer m_TimerSnakeMove;
    // animation timer
    Timer m_AnimTimerSnakeBlink;
    // index value for random blinks
    uint8_t m_AnimBlinkValue = 0;

    //----------------------------------------------------
    // CONST
    //----------------------------------------------------
    static constexpr float C_ANIM_BLINK_TIME = 0.15f;
    // some 'random' animation timings
    static constexpr uint8_t C_ANIM_BLINKWAIT[32] = {5,3,4,5,2,5,4,1,2,5,6,5,2,3,4,5,4,2,4,2,3,4,4,5,2,3,5,1,1,4,5,2};

private:
    void m_PollInputs();

    // setup for tile rendering
    Vector2 m_TileSizeBase() const;

    // custom draws
    void m_DrawSnake();
    void m_DrawSnakeHead(const Texture2D& snakeHead, float tileSize, const Vector2& textureSize);
    void m_DrawGameArea();
    void m_DrawDebugSnakePos();

    // snake logic
    void m_SnakeAdvance(float deltaTime);

    // food logic
    void m_PositionFood();
};
