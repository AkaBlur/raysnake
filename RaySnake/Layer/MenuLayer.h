#pragma once

#include "RaySnake.h"

class MenuLayer
{
public:
	MenuLayer(RaySnake::GameStack& mainGameStack) :
		m_LastKeyPressed(0), m_MenuIndex(0), m_GameStack(mainGameStack) { };

public:
	void Draw(float timestep);
	void OnUpdate(float timestep);

private:
	void m_PollInputs();

private:
	// last key pressed
	int m_LastKeyPressed;
	// menu index
	int m_MenuIndex;

	// game state of main game
	RaySnake::GameStack& m_GameStack;

};
