#include "MenuChoice.h"

#include <sstream>

#include "Graphics/DrawGeometry.h"

void MenuChoice::Draw(float scaleFactor)
{
    Geometry::DrawTextboxCenteredMultiMenu(m_MenuDrawText, 24, m_MenuItemPositions, scaleFactor);

    for (int i = 1; i < m_MenuItemPositions.size(); i++)
    {
        Geometry::DrawKey(m_MenuItemPositions[i], 24 * scaleFactor, m_MenuItems[i - 1].KeyCode.c_str(), BLACK, m_MenuItems[i - 1].KeyCode.length());
    }
}

void MenuChoice::AddChoice(const char* text, const char* key)
{
    m_MenuItems.emplace_back(text, key);
    m_UpdateMenuString();
}

void MenuChoice::m_UpdateMenuString()
{
    std::stringstream MenuStringStream;
    MenuStringStream << m_MenuTitle;

    // optional separator for menu items from title
    if (!m_MenuItems.empty())
        MenuStringStream << '\n';

    for (const MenuItem& CurrentItem : m_MenuItems)
    {
        MenuStringStream << '\t' << '\t';
        MenuStringStream << CurrentItem.Text;

        if (CurrentItem.Text != m_MenuItems.back().Text)
            MenuStringStream << '\n';
    }

    m_MenuDrawText = MenuStringStream.str();

}
