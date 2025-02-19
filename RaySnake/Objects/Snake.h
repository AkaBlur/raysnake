//
// Created by filip on 08.02.25.
//

#ifndef SNAKE_H
#define SNAKE_H
#include <array>
#include <cstdint>
#include <random>
#include <vector>

#include "raylib.h"

struct SnakePart
{
    SnakePart() :
        IsHead(true) {};
    SnakePart(SnakePart* nextPartToHead) :
        nextPartToHead(nextPartToHead) {};
    SnakePart(uint8_t x, uint8_t y, uint8_t rotation, SnakePart* nextPartToHead) :
        x(x), y(y), rotation(rotation), nextPartToHead(nextPartToHead) {};

    uint8_t x = 0;
    uint8_t y = 0;
    // part rotation, multiple of 90 degrees clock-wise
    uint8_t rotation = 1;

    // part is the snakes' head
    bool IsHead = false;
    // part is the snakes' tail
    bool IsTail = false;
    // link to the next part in direction towards head
    // nullptr when part is head
    SnakePart* nextPartToHead = nullptr;
};

class Snake {
public:
    Snake(uint8_t fieldSizeX, uint8_t fieldSizeY);
    
    Snake& operator=(Snake& copy);
    
    Snake(const Snake& copy) = delete;
    Snake(Snake&& move) = delete;
public:
    const SnakePart& PartHead() const { return m_PartHead; };
    const std::vector<SnakePart>& PartBody() const { return m_PartBody; };
    const SnakePart& PartTail() const { return m_PartTail; };

public:
    enum class SnakeState
    {
        NORMAL = 0,
        FOOD = 1,
        MINE = 10,
        DEAD = 20
    };

public:
    void RotateHead(uint8_t rotation) { m_PartHead.rotation = rotation; };
    SnakeState AdvanceSnake(const Vector2& foodPos);

    uint32_t GetLength() const { return (m_PartBody.size() + 2); };

    const std::array<uint8_t, 65536>& GetSnakePositions() const { return m_Playfield; };

private:
    // game field size
    uint8_t m_FieldLength, m_FieldWidth;
    // game field, maximum size = uint8_t ** 2
    // may not be the most memory efficient storage but allows pretty dynamic position calculations
    // stores snake positions,
    // 0 - no snake
    // 1 - snake
    std::array<uint8_t, 65536> m_Playfield;

    // snake parts
    SnakePart m_PartHead;
    std::vector<SnakePart> m_PartBody;
    SnakePart m_PartTail;

    // current state of the snake
    SnakeState m_SnakeState = SnakeState::NORMAL;

    inline Vector2 m_MoveBaseVector() const
    {
        switch (m_PartHead.rotation)
        {
            case 0:
                return {0.0f, -1.0f};
                break;

            case 1:
                return {1.0f, 0.0f};
                break;

            case 2:
                return {0.0f, 1.0f};
                break;

            case 3:
                return {-1.0f, 0.0f};
                break;

            default:
                return {0.0f, 0.0f};
                break;

        }
    }
};



#endif //SNAKE_H
