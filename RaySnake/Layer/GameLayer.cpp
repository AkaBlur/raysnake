#include "GameLayer.h"

#include <iostream>
#include <ostream>

#include "raylib.h"
#include "raymath.h"
#include "Graphics/DrawGeometry.h"

void GameLayer::OnUpdate(float deltaTime)
{
    // input
    m_PollInputs();

    // animations
    // snake blink
    m_AnimTimerSnakeBlink.Tick(deltaTime);

    // update window size
    m_WindowArea.x = GetScreenWidth();
    m_WindowArea.y = GetScreenHeight();

    // update play area size
    Vector2 TileSize = m_TileSizeBase();
    m_GameArea = Vector2Scale(TileSize, m_GameSettings.BoardSize);

    // update to corner of play field
    Vector2 OffsetTotal = Vector2Subtract(m_WindowArea, m_GameArea);
    m_GameAreaTopCorner = Vector2Scale(OffsetTotal, 0.5f);

    // check for pause
    if (m_LastKeyPressed == KEY_ESCAPE)
    {
        switch (m_GameState)
        {
            case GameState::Playing:
                m_GameState = GameState::Paused;
                break;

            case GameState::Paused:
                m_GameState = GameState::Playing;

            default:
                break;

        }

    }

    // advance
    switch (m_GameState)
    {
    case GameState::Playing:
        {
            // rotate
            if (m_LastKeyPressed == KEY_W)
            {
                m_Snake.RotateHead(0);

            } else if (m_LastKeyPressed == KEY_S)
            {
                m_Snake.RotateHead(2);

            } else if (m_LastKeyPressed == KEY_A)
            {
                m_Snake.RotateHead(3);

            } else if (m_LastKeyPressed == KEY_D)
            {
                m_Snake.RotateHead(1);

            }

            m_SnakeAdvance(deltaTime);
            break;
        }

    case GameState::Paused:
        if (m_LastKeyPressed == KEY_Q)
            m_GameStack.GameMode = RaySnake::GameMode::EndScreen;
        break;

        m_DeadMenu.AddChoice("Restart", "R");
        m_DeadMenu.AddChoice("Return to Menu", "Esc");
        m_DeadMenu.AddChoice("Exit", "Q");

    case GameState::GameOver:
        switch (m_LastKeyPressed)
        {
        case KEY_R:
            Reset();
            break;
            
        case KEY_ESCAPE:
            Reset();
            m_GameStack.GameMode = RaySnake::GameMode::MainMenu;
            break;

        case KEY_Q:
            m_GameStack.GameMode = RaySnake::GameMode::EndScreen;
            break;
            
        default:
            break;
        }

        break;
    default:
        break;

    }

}

void GameLayer::Init()
{
    m_ImageAssets.SnakeHead = LoadImage("res/snake_head.png");
    m_ImageAssets.SnakeHeadBlink = LoadImage("res/snake_head_blink.png");
    m_ImageAssets.SnakeHeadEat = LoadImage("res/snake_head_eat.png");
    m_ImageAssets.SnakeBody = LoadImage("res/snake_body.png");
    m_ImageAssets.SnakeTail = LoadImage("res/snake_tail.png");
    m_ImageAssets.Food = LoadImage("res/food.png");
    m_ImageAssets.Mine = LoadImage("res/mine.png");

    m_Textures.SnakeHead = LoadTextureFromImage(m_ImageAssets.SnakeHead);
    m_Textures.SnakeHeadBlink = LoadTextureFromImage(m_ImageAssets.SnakeHeadBlink);
    m_Textures.SnakeHeadEat = LoadTextureFromImage(m_ImageAssets.SnakeHeadEat);
    m_Textures.SnakeBody = LoadTextureFromImage(m_ImageAssets.SnakeBody);
    m_Textures.SnakeTail = LoadTextureFromImage(m_ImageAssets.SnakeTail);
    m_Textures.Food = LoadTextureFromImage(m_ImageAssets.Food);
    m_Textures.Mine = LoadTextureFromImage(m_ImageAssets.Mine);

    SetExitKey(0);

    m_PositionFood();

}

void GameLayer::Reset()
{
    m_GameSettings = GameSettings();
    Snake newSnake(m_GameSettings.BoardSize, m_GameSettings.BoardSize);
    m_Snake = newSnake;

    m_GameState = GameState::Playing;

    m_FoodCount = 0;
    m_AnimBlinkValue = 0;
    m_LastKeyPressed = 0;

    m_PositionFood();

}

void GameLayer::Draw()
{
    // playfield
    m_DrawGameArea();
    // snake
    m_DrawSnake();
    // food
    DrawTextureEx(m_Textures.Food, Vector2Add(Vector2Scale(m_FoodPos, m_TileSizeBase().x), m_GameAreaTopCorner), 0,
                  m_GameStack.GameScaling, WHITE);

    //m_DrawDebugSnakePos();

    // menu and sidebar
    switch (m_GameState)
    {
        case GameState::Playing:
            DrawText("Let's go", 25 * m_GameStack.GameScaling, 20 * m_GameStack.GameScaling, 18 * m_GameStack.GameScaling, BLACK);
            break;

        case GameState::Paused:
            m_PauseMenu.Draw(m_GameStack.GameScaling);
            break;

        case GameState::GameOver:
            m_DeadMenu.Draw(m_GameStack.GameScaling);
            break;

        default:
            break;
    }

}

void GameLayer::m_PollInputs()
{
    int KeyBuffer = GetKeyPressed();
    if (KeyBuffer == 0)
    {
        m_LastKeyPressed = 0;
        return;

    }

    while (KeyBuffer != 0)
    {
        m_LastKeyPressed = KeyBuffer;
        KeyBuffer = GetKeyPressed();

    }

}

Vector2 GameLayer::m_TileSizeBase() const
{
    float BaseTileSize = (float) (m_ImageAssets.SnakeHead.width) * m_GameStack.GameScaling;
    return { BaseTileSize, BaseTileSize };
}

void GameLayer::m_SnakeAdvance(float deltaTime)
{
    float SnakeMoveTime = m_TimerSnakeMove.Tick(deltaTime);

    if (SnakeMoveTime >= m_GameSettings.SnakeMoveTime)
    {
        m_TimerSnakeMove.Reset();

        switch (m_Snake.AdvanceSnake(m_FoodPos))
        {
        case Snake::SnakeState::NORMAL:
            break;
        case Snake::SnakeState::FOOD:
            {
                m_PositionFood();
                m_FoodCount++;

                float MoveSpeedMod = pow(C_SNAKE_MOVESPEED_MOD, (float) ((int) m_FoodCount / 5));
                m_GameSettings.SnakeMoveTime = C_SNAKE_MOVESPEED_BASE * MoveSpeedMod;

                break;

            }
        case Snake::SnakeState::MINE:
        case Snake::SnakeState::DEAD:
            m_GameState = GameState::GameOver;
            break;
        default:
            break;
        }
    }
}

void GameLayer::m_PositionFood()
{
    uint32_t SnakeLength = m_Snake.GetLength();
    uint32_t RandomPos = Random::GetRandomNumber(m_RandomEngine,
                                                 (m_GameSettings.BoardSize * m_GameSettings.BoardSize - SnakeLength - 1));

    const std::array<uint8_t, 65536>& SnakePositions = m_Snake.GetSnakePositions();

    uint32_t MovedPos = 0;
    RandomPos += SnakePositions[MovedPos];
    while (RandomPos > 0)
    {
        MovedPos++;
        RandomPos--;

        RandomPos += SnakePositions[MovedPos];

    }

    uint32_t MovedX = MovedPos % (int) m_GameSettings.BoardSize;
    uint32_t MovedY = (int) ((MovedPos - MovedX) / m_GameSettings.BoardSize);

    m_FoodPos.x = MovedX;
    m_FoodPos.y = MovedY;

}

void GameLayer::m_DrawSnake()
{
    float TileScale = m_TileSizeBase().x;
    Vector2 TextureSize = {(float) m_ImageAssets.SnakeHead.width, (float) m_ImageAssets.SnakeHead.height};

    // head, takes animation into account
    float BlinkTime = m_AnimTimerSnakeBlink.Time();
    if (BlinkTime >= (float) C_ANIM_BLINKWAIT[m_AnimBlinkValue] && BlinkTime < (float) (C_ANIM_BLINKWAIT[m_AnimBlinkValue] + C_ANIM_BLINK_TIME))
    {
        m_DrawSnakeHead(m_Textures.SnakeHeadBlink, TileScale, TextureSize);

    } else if (BlinkTime >= (float) (C_ANIM_BLINKWAIT[m_AnimBlinkValue] + C_ANIM_BLINK_TIME))
    {
        m_AnimTimerSnakeBlink.Reset();
        m_AnimBlinkValue++;
        m_AnimBlinkValue %= 32;
        m_DrawSnakeHead(m_Textures.SnakeHead, TileScale, TextureSize);

    } else
    {
        m_DrawSnakeHead(m_Textures.SnakeHead, TileScale, TextureSize);
    }

    // body
    for (SnakePart bodyPart : m_Snake.PartBody())
    {
        Vector2 BodyTilePosition = {(float) bodyPart.x, (float) bodyPart.y};
        Geometry::DrawTextureRotated(m_Textures.SnakeBody, TextureSize,
                                     Vector2Add(Vector2Scale(Vector2SubtractValue(BodyTilePosition, 0.5f), TileScale),
                                                m_GameAreaTopCorner), bodyPart.rotation, m_GameStack.GameScaling);
    }

    // tail
    Vector2 TailPosition = { (float) m_Snake.PartTail().x, (float) m_Snake.PartTail().y };
    Geometry::DrawTextureRotated(m_Textures.SnakeTail, TextureSize,
                                 Vector2Add(Vector2Scale(Vector2SubtractValue(TailPosition, 0.5f), TileScale),
                                            m_GameAreaTopCorner), m_Snake.PartTail().rotation, m_GameStack.GameScaling);

}

void GameLayer::m_DrawSnakeHead(const Texture2D& snakeHead, float tileSize, const Vector2& textureSize)
{
    // head
    Vector2 HeadTilePosition = { (float) m_Snake.PartHead().x, (float) m_Snake.PartHead().y };
    Geometry::DrawTextureRotated(snakeHead,
                                 textureSize,
                                 Vector2Add(Vector2Scale(Vector2SubtractValue(HeadTilePosition, 0.5f), tileSize), m_GameAreaTopCorner),
                                 m_Snake.PartHead().rotation, m_GameStack.GameScaling);
}

void GameLayer::m_DrawGameArea()
{
    Vector2 Screen = {(float) GetScreenWidth(), (float) GetScreenHeight()};
    Vector2 TileSize = m_TileSizeBase();

    float TotalBoardSizeQuad = m_GameSettings.BoardSize * TileSize.x;
    Vector2 BoardOffset = Vector2Scale(Vector2Subtract(Screen, {TotalBoardSizeQuad, TotalBoardSizeQuad}), 0.5f);

    Color ColorMap[2] = {LIGHTGRAY, GRAY};

    // flag for evenness
    int IsEven = (m_GameSettings.BoardSize + 1) % 2;
    for (int i = 0; i < m_GameSettings.BoardSize; i++) {
        for (int j = 0; j < m_GameSettings.BoardSize; j++) {
            // offsets all odd column indices when board has even size
            int NormedIdx = (i * (m_GameSettings.BoardSize + IsEven) + j) % 2;

            Vector2 TilePos = Vector2Add({TileSize.x * i, TileSize.y * j}, BoardOffset);
            DrawRectangle(TilePos.x, TilePos.y, TileSize.x, TileSize.y, ColorMap[NormedIdx]);

        }

    }
}

void GameLayer::m_DrawDebugSnakePos()
{
    uint32_t MaxTileCount = m_GameSettings.BoardSize * m_GameSettings.BoardSize;

    for (uint32_t i = 0; i < MaxTileCount; i++)
    {
        if (m_Snake.GetSnakePositions()[i] == 1)
            continue;

        Vector2 Pos = { (float) (i % m_GameSettings.BoardSize), (float) ((uint32_t) i / m_GameSettings.BoardSize)};

        Vector2 StartPosBase = { Pos.x * m_TileSizeBase().x, Pos.y * m_TileSizeBase().y };
        Vector2 StartPos = Vector2Add(StartPosBase, m_GameAreaTopCorner);

        DrawLineEx(StartPos, Vector2Add(StartPos, {12, 12}), 1.5, RED);

    }
}
