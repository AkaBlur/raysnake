#include "DrawGeometry.h"

#include <algorithm>
#include <string>

#include "raymath.h"

namespace Geometry
{
    static std::vector<Vector2> emptyPositions;

    static void m_DrawMenuBase(const std::string& text, uint32_t textsize, bool drawMenu, std::vector<Vector2>& menuItemPositions, float scaleFactor)
    {
        //---------------------------
        // SETUPS
        //---------------------------
        Vector2 ScreenSize = {(float) GetScreenWidth(), (float) GetScreenHeight()};
        uint32_t FontSize = textsize * scaleFactor;
        uint32_t LineHeight = textsize * scaleFactor * 1.2f;
        // measure newline count
        uint32_t LineCount = 1;
        std::string::const_iterator LineIt = std::find(text.begin(), text.end(), '\n');
        while (LineIt != text.end())
        {
            LineCount++;
            LineIt++;
            LineIt = std::find(LineIt, text.end(), '\n');
        }
        menuItemPositions.clear();

        //---------------------------
        // GEOMETRY
        //---------------------------
        // input text is newline separated, detected by MeasureText
        int TextLength = MeasureText(text.c_str(), FontSize);
        Vector2 Textbox = {TextLength * 1.2f, FontSize * 1.35f * LineCount};

        float RectPosX = ScreenSize.x * 0.5f - Textbox.x * 0.5f;
        float RectPosY = ScreenSize.y * 0.5f - Textbox.y * 0.5f;

        DrawRectangleRounded({RectPosX, RectPosY, Textbox.x, Textbox.y}, 0.05f, 1, LIGHTBLUE);
        DrawRectangleRoundedLinesEx({RectPosX, RectPosY, Textbox.x, Textbox.y}, 0.05f, 1, (2.0f * scaleFactor), BLUE);

        Vector2 TextPos = {
            ScreenSize.x * 0.5f - TextLength * 0.5f,
            ScreenSize.y * 0.5f - LineHeight * LineCount * 0.5f
        };

        //---------------------------
        // DRAW SETUP
        //---------------------------
        std::string TextDrawn(text);
        int LineIndex = 0;
        std::string::iterator SubStrPosition = TextDrawn.begin();
        std::string::iterator NextStrPosition = std::find(TextDrawn.begin(), TextDrawn.end(), '\n');

        Vector2 TextPosDraw = {TextPos.x, (TextPos.y + LineHeight * LineIndex)};
        menuItemPositions.push_back(TextPosDraw);

        // Menu Choices Setup
        if (drawMenu)
        {
            *NextStrPosition = '\0';

            DrawText(&(*SubStrPosition), TextPosDraw.x, TextPosDraw.y, FontSize, BLUE);

            Vector2 UnderlinePos = Vector2Add(TextPos, {0.0f, (float) (textsize * scaleFactor)});
            DrawLineEx(UnderlinePos, Vector2Add(UnderlinePos, {(float) TextLength, 0}), 2.0f, BLUE);

            LineIndex++;
            NextStrPosition++;
            SubStrPosition = NextStrPosition;
            NextStrPosition = std::find(SubStrPosition, TextDrawn.end(), '\n');

        }

        //---------------------------
        // DRAW
        //---------------------------
        while (LineIndex < LineCount)
        {
            *NextStrPosition = '\0';

            TextPosDraw = {TextPos.x, (TextPos.y + LineHeight * LineIndex)};
            menuItemPositions.push_back(TextPosDraw);

            DrawText(&(*SubStrPosition), TextPosDraw.x, TextPosDraw.y, FontSize, BLACK);

            LineIndex++;
            NextStrPosition++;
            SubStrPosition = NextStrPosition;
            NextStrPosition = std::find(SubStrPosition, TextDrawn.end(), '\n');

        }
    }

    void DrawTriangleEquilateral(const Vector2 &midpoint, float length, float rotation, Color color)
    {
        Vector2 TopVec = {
            midpoint.x, midpoint.y + (length * 0.5773502f)
        };
        Vector2 BaseVec = Vector2Subtract(TopVec, midpoint);
        BaseVec = Vector2Rotate(BaseVec, rotation);
        TopVec = Vector2Add(BaseVec, midpoint);

        Vector2 RightVec = Vector2Rotate(BaseVec, (2.0f * PI / 3.0f));
        RightVec = Vector2Add(RightVec, midpoint);

        Vector2 LeftVec = Vector2Rotate(BaseVec, (4.0f * PI / 3.0f));
        LeftVec = Vector2Add(LeftVec, midpoint);

        DrawTriangle(TopVec, LeftVec, RightVec, color);

    }

    void DrawKey(const Vector2 &position, float size, const char* key, Color color, uint32_t keycodeLength)
    {
        float Offset = size * 0.15f;
        float TextSize = size / keycodeLength;

        DrawRectangleRoundedLinesEx({position.x, position.y, size, size}, 0.5f, 1, 3.0f, BLACK);
        DrawText(key, (position.x + Offset), (position.y + Offset * 0.5f), TextSize, color);

    }

    void DrawTextMultiline(const Vector2& position, const std::string &text, int textsize, float scaleFactor, Color color)
    {
        uint32_t FontSize = textsize * scaleFactor;
        uint32_t LineHeight = textsize * scaleFactor * 1.2f;
        // measure newline count
        uint32_t LineCount = 1;
        std::string::const_iterator LineIt = std::find(text.begin(), text.end(), '\n');
        while (LineIt != text.end())
        {
            LineCount++;
            LineIt++;
            LineIt = std::find(LineIt, text.end(), '\n');
        }

        std::string TextDrawn(text);
        int LineIndex = 0;
        std::string::iterator SubStrPosition = TextDrawn.begin();
        std::string::iterator NextStrPosition = std::find(TextDrawn.begin(), TextDrawn.end(), '\n');
        
        while (LineIndex < LineCount)
        {
            *NextStrPosition = '\0';

            Vector2 TextPosDraw = {position.x, (position.y + LineHeight * LineIndex)};

            DrawText(&(*SubStrPosition), TextPosDraw.x, TextPosDraw.y, FontSize, color);

            LineIndex++;
            NextStrPosition++;
            SubStrPosition = NextStrPosition;
            NextStrPosition = std::find(SubStrPosition, TextDrawn.end(), '\n');

        }
    }

    void DrawArrow(Vector2 startPosition, float length, float rotation, Color color)
    {
        Vector2 EndPos = {
            startPosition.x + length * cos(rotation * PI / 180.0f), startPosition.y + length * sin(rotation * PI / 180.0f)};
        DrawLineEx(startPosition, EndPos, 3.0f, color);
        float TipLength = length * 0.25f;
        Vector2 Tip1 = {
        EndPos.x + TipLength * cos((rotation + 150) * PI / 180.0f), EndPos.y + TipLength * sin((rotation + 150) * PI / 180.0f)};
        Vector2 Tip2 = {
            EndPos.x + TipLength * cos((rotation - 150) * PI / 180.0f), EndPos.y + TipLength * sin((rotation - 150) * PI / 180.0f)};
        DrawLineEx(EndPos, Tip1, 3.0f, color);
        DrawLineEx(EndPos, Tip2, 3.0f, color);

    }

    void DrawTextboxCentered(const char *text, int textsize, float scaleFactor)
    {
        Vector2 ScreenSize = {(float) GetScreenWidth(), (float) GetScreenHeight()};
        int FontSize = textsize * scaleFactor;
        int TextLength = MeasureText(text, FontSize);
        Vector2 Textbox = {TextLength * 1.2f, FontSize * 1.15f};

        float RectPosX = ScreenSize.x * 0.5f - Textbox.x * 0.5f;
        float RectPosY = ScreenSize.y * 0.5f - Textbox.y * 0.5f;

        DrawRectangleRounded({RectPosX, RectPosY, Textbox.x, Textbox.y}, 0.05f, 1, LIGHTBLUE);
        DrawRectangleRoundedLinesEx({RectPosX, RectPosY, Textbox.x, Textbox.y}, 0.05f, 1, (2.0f * scaleFactor), BLUE);

        float TextPosX = ScreenSize.x * 0.5f - TextLength * 0.5f;
        float TextPosY = ScreenSize.y * 0.5f - FontSize * 0.5f;

        DrawText(text, TextPosX, TextPosY, FontSize, BLACK);

    }

    void DrawTextboxCenteredMultiline(const std::string& text, int textsize, float scaleFactor)
    {
        m_DrawMenuBase(text, textsize, false, emptyPositions, scaleFactor);
    }

    void DrawTextboxCenteredMultiMenu(const std::string& text, int textsize, std::vector<Vector2>& menuItemPositions, float scaleFactor)
    {
        m_DrawMenuBase(text, textsize, true, menuItemPositions, scaleFactor);
    }

    void DrawTextureRotated(const Texture2D& texture, const Vector2& textureSize, const Vector2& position, uint8_t rotation, float scaling)
    {
        Vector2 BaseOffsetVector = Vector2Scale(
            Vector2Rotate(Vector2Scale(textureSize, 0.5f), (rotation * PI * 0.5f)), scaling);
        DrawTextureEx(texture, Vector2Subtract(position, BaseOffsetVector), rotation * 90.0f,
                      scaling, WHITE);
    }

    void m_DrawMenuBase(uint32_t LineIndexStart, uint32_t LineCount, uint32_t LineHeight, uint32_t FontSize,
        std::string& TextDrawn, std::string::iterator& SubStrPosition, std::string::iterator& NextStrPosition,
        const Vector2& TextPos)
    {

    }

}
