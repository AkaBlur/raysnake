#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "raylib.h"

namespace Geometry
{
    // Basic shapes
    void DrawTriangleEquilateral(const Vector2& midpoint, float length, float rotation, Color color);
    void DrawArrow(Vector2 startPosition, float length, float rotation, Color color);

    // keyboard-key
    void DrawKey(const Vector2& position, float size, const char* key, Color color, uint32_t keycodeLength = 1);

    // raw text
    void DrawTextMultiline(const Vector2& position, const std::string& text, int textsize, float scaleFactor, Color color);
    
    // textboxes
    void DrawTextboxCentered(const char* text, int textsize, float scaleFactor);
    void DrawTextboxCenteredMultiline(const std::string& text, int textsize, float scaleFactor);
    void DrawTextboxCenteredMultiMenu(const std::string& text, int textsize, std::vector<Vector2>& menuItemPositions, float scaleFactor);

    // textures
    void DrawTextureRotated(const Texture2D& texture, const Vector2& textureSize, const Vector2& position, uint8_t rotation, float scaling);

    static constexpr Color LIGHTBLUE = {230, 230, 255, 255};
}
