#pragma once

#include "RaySnake.h"

class MenuLayer
{
public:
	MenuLayer(RaySnake::AppSettings& mainSettings) :
		m_LastKeyPressed(0), m_MenuIndex(0), m_AppSettings(mainSettings) { };

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

	// settings of main application
	RaySnake::AppSettings& m_AppSettings;

};
