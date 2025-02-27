#pragma once

class Timer
{
public:
    Timer() :
        m_TotalTimeSeconds(0.0f) {};

public:
    float Tick(float deltaTime)
    {
        m_TotalTimeSeconds += deltaTime;
        return m_TotalTimeSeconds;

    };

    void Reset() { m_TotalTimeSeconds = 0.0f; };

    float Time() const { return m_TotalTimeSeconds; };

private:
    float m_TotalTimeSeconds;

};
