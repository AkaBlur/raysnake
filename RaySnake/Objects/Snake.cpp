//
// Created by filip on 08.02.25.
//

#include "Snake.h"

#include <iostream>
#include <ostream>

#include "raymath.h"

Snake::Snake(uint8_t fieldSizeX, uint8_t fieldSizeY) :
    m_FieldWidth(fieldSizeX), m_FieldLength(fieldSizeY)
{
    // empty playfield
    m_Playfield.fill(0);

    // field initialize with maximum length for body parts
    m_PartBody.reserve(fieldSizeX * fieldSizeY);

    uint8_t SnakeHeadX = (uint8_t) (fieldSizeX * 0.3f);
    uint8_t SnakeHeadY = (uint8_t) (fieldSizeY * 0.3f);

    // snake head starts in upper third
    m_PartHead.x = SnakeHeadX;
    m_PartHead.y = SnakeHeadY;
    m_PartHead.rotation = 1;
    m_PartHead.IsHead = true;
    m_PartHead.IsTail = false;
    // set head
    m_Playfield[(SnakeHeadX - 1 ) + (SnakeHeadY - 1) * m_FieldLength] = 1;

    // snake body part is one behind, initially one body part
    m_PartBody.emplace_back(SnakeHeadX - 1, SnakeHeadY, m_PartHead.rotation, &m_PartHead);
    // set body
    m_Playfield[(SnakeHeadX - 2) + (SnakeHeadY - 1) * m_FieldLength] = 1;

    // snake tail part is at the end
    m_PartTail.x = SnakeHeadX - 2;
    m_PartTail.y = SnakeHeadY;
    m_PartTail.rotation = m_PartHead.rotation;
    m_PartTail.IsHead = false;
    m_PartTail.IsTail = true;
    m_PartTail.nextPartToHead = &m_PartBody.back();
    // set tail
    m_Playfield[(SnakeHeadX - 3) + (SnakeHeadY - 1) * m_FieldLength] = 1;

}

Snake& Snake::operator=(Snake &copy)
{
    this->m_FieldLength = copy.m_FieldLength;
    this->m_FieldWidth = copy.m_FieldWidth;

    this->m_Playfield = copy.m_Playfield;

    // construct tail
    this->m_PartTail.x = copy.m_PartTail.x;
    this->m_PartTail.y = copy.m_PartTail.y;
    this->m_PartTail.rotation = copy.m_PartTail.rotation;
    this->m_PartTail.IsHead = false;
    this->m_PartTail.IsTail = true;

    // construct body parts
    // there is always one body part
    this->m_PartBody.clear();
    this->m_PartBody.reserve(this->m_FieldLength * this->m_FieldWidth);
    SnakePart* nextCopyBodyPart = copy.m_PartTail.nextPartToHead;

    while (!nextCopyBodyPart->IsHead)
    {
        this->m_PartBody.emplace_back(nextCopyBodyPart->x, nextCopyBodyPart->y, nextCopyBodyPart->rotation, nullptr);

        nextCopyBodyPart = nextCopyBodyPart->nextPartToHead;
    }

    // construct head
    this->m_PartHead.x = copy.m_PartHead.x;
    this->m_PartHead.y = copy.m_PartHead.y;
    this->m_PartHead.rotation = copy.m_PartHead.rotation;
    this->m_PartHead.IsHead = true;
    this->m_PartHead.IsTail = false;
    this->m_PartHead.nextPartToHead = nullptr;

    // update body part pointers
    for (std::vector<SnakePart>::iterator it = this->m_PartBody.begin(); it != this->m_PartBody.end(); it++)
    {
        if ((it + 1) != this->m_PartBody.end())
        {
            SnakePart& nextPart = *(it + 1);
            it->nextPartToHead = &nextPart;

        } else
        {
            it->nextPartToHead = &m_PartHead;
        }

    }

    this->m_SnakeState = copy.m_SnakeState;

    return *this;
    
}

Snake::SnakeState Snake::AdvanceSnake(const Vector2& foodPos)
{
    // check for food first
    // advance head
    Vector2 NewHeadPosition = { (float) m_PartHead.x, (float) m_PartHead.y };
    NewHeadPosition = Vector2Add(NewHeadPosition, m_MoveBaseVector());

    // food-related actions
    SnakePart* atlasPart = m_PartTail.nextPartToHead;
    bool IsFed = false;
    if (NewHeadPosition.x == (foodPos.x + 1) && NewHeadPosition.y == (foodPos.y + 1))
    {
        IsFed = true;
        // get the last part of the body
        while (atlasPart->nextPartToHead->IsHead == false)
            atlasPart = atlasPart->nextPartToHead;

    } else
    {
        // snake advances from back to front
        SnakePart* nextPart = m_PartTail.nextPartToHead;
        // remove tail
        m_Playfield[(m_PartTail.x - 1) + (m_PartTail.y - 1) * m_FieldLength] = 0;
        // advance tail
        m_PartTail.x = nextPart->x;
        m_PartTail.y = nextPart->y;
        m_PartTail.rotation = nextPart->rotation;

        // advance body
        SnakePart* currentPart = nextPart;
        nextPart = currentPart->nextPartToHead;
        while (!currentPart->IsHead)
        {
            currentPart->x = nextPart->x;
            currentPart->y = nextPart->y;
            currentPart->rotation = nextPart->rotation;

            currentPart = nextPart;
            nextPart = nextPart->nextPartToHead;
        }
    }

    // basic overshoot checks
    // game area is a multiplier for rendering
    // -> ranges from 1 to maximum tile amount
    if (NewHeadPosition.x < 1.0f || NewHeadPosition.x > m_FieldWidth)
        return SnakeState::DEAD;

    else if (NewHeadPosition.y < 1.0f || NewHeadPosition.y > m_FieldLength)
        return SnakeState::DEAD;

    SnakePart newHead;
    newHead.x = (uint8_t) NewHeadPosition.x;
    newHead.y = (uint8_t) NewHeadPosition.y;
    newHead.rotation = m_PartHead.rotation;
    newHead.IsHead = true;
    newHead.IsTail = false;

    // tail collision
    if (newHead.x == m_PartTail.x && newHead.y == m_PartTail.y)
        return SnakeState::DEAD;

    // body part collision
    for (const SnakePart& bodyPart : m_PartBody)
    {
        if (bodyPart.x == newHead.x && bodyPart.y == newHead.y)
            return SnakeState::DEAD;
    }

    // advance head
    SnakePart oldHead = m_PartHead;
    m_PartHead = newHead;
    // set head
    m_Playfield[(m_PartHead.x - 1) + (m_PartHead.y - 1) * m_FieldLength] = 1;
    if (IsFed)
    {
        m_PartBody.emplace_back(oldHead.x, oldHead.y, oldHead.rotation, &m_PartHead);
        atlasPart->nextPartToHead = &m_PartBody.back();

        return SnakeState::FOOD;

    }

    return SnakeState::NORMAL;

}
