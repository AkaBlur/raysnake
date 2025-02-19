#include <iostream>

#include "raylib.h"

#include "MenuLayer.h"
#include "Graphics/DrawGeometry.h"


void MenuLayer::Draw(float timestep)
{
	// CAPTION
	Vector2 BasePos = {50 * m_GameStack.GameScaling, 60 * m_GameStack.GameScaling};
	DrawText("TASG", BasePos.x, BasePos.y, 64 * m_GameStack.GameScaling, DARKGRAY);

	Vector2 MenuPos = {BasePos.x + 60 * m_GameStack.GameScaling, BasePos.y + 80 * m_GameStack.GameScaling};
	int MenuYOffset = 30 * m_GameStack.GameScaling;
	DrawText("Start", MenuPos.x, MenuPos.y, 24 * m_GameStack.GameScaling, DARKGREEN);
	DrawText("Exit", MenuPos.x, (MenuPos.y + MenuYOffset), 24 * m_GameStack.GameScaling, DARKPURPLE);

	Vector2 MenuTriangle = {90.0f * m_GameStack.GameScaling, (150.0f + 30.0f * m_MenuIndex) * m_GameStack.GameScaling};
	Geometry::DrawTriangleEquilateral(MenuTriangle, 14 * m_GameStack.GameScaling, (PI * 1.5f), RED);

	float ControlSize = 36.0f * m_GameStack.GameScaling;
	float MaxFontSize = MeasureText("W", ControlSize);
	Vector2 ControlPos = {MenuPos.x + 320 * m_GameStack.GameScaling, MenuPos.y + 240 * m_GameStack.GameScaling};
	int ControlOffsetX = 32 * m_GameStack.GameScaling;
	int ControlOffsetY = 32 * m_GameStack.GameScaling;
	float ControlPadding = (2 * m_GameStack.GameScaling);

	Geometry::DrawKey({ControlPos.x, ControlPos.y}, MaxFontSize, "W", BLACK);
	Geometry::DrawKey({(ControlPos.x - ControlOffsetX + ControlPadding), ControlPos.y + ControlOffsetY}, MaxFontSize, "A", BLACK);
	Geometry::DrawKey({(ControlPos.x + ControlPadding), ControlPos.y + ControlOffsetY}, MaxFontSize, "S", BLACK);
	Geometry::DrawKey({(ControlPos.x + ControlOffsetX + ControlPadding), ControlPos.y + ControlOffsetY}, MaxFontSize, "D", BLACK);
	DrawText("Move", (ControlPos.x + (65 * m_GameStack.GameScaling)), (ControlPos.y + (15 * m_GameStack.GameScaling)), (32 * m_GameStack.GameScaling), BLACK);

}

void MenuLayer::OnUpdate(float timestep)
{
	m_PollInputs();

	if (m_LastKeyPressed == KeyboardKey::KEY_W) {
		m_MenuIndex--;

		if (m_MenuIndex < 0)
			m_MenuIndex = 0;

	} else if (m_LastKeyPressed == KeyboardKey::KEY_S) {
		m_MenuIndex++;

		if (m_MenuIndex >= 2)
			m_MenuIndex = 1;

	} else if (m_LastKeyPressed == KeyboardKey::KEY_ENTER) {
		switch (m_MenuIndex) {
			case 0:
				m_GameStack.GameMode = RaySnake::GameMode::Playing;
				break;

			case 1:
				m_GameStack.GameMode = RaySnake::GameMode::EndScreen;
				break;

			default:
				break;
		}

	}

	m_LastKeyPressed = 0;

}

void MenuLayer::m_PollInputs()
{
	int KeyBuffer = GetKeyPressed();
	
	while (KeyBuffer != 0)
	{
		m_LastKeyPressed = KeyBuffer;
		KeyBuffer = GetKeyPressed();

	}

}
