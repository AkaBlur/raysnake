//
// Created by filip on 15.02.25.
//

#ifndef MENUCHOICE_H
#define MENUCHOICE_H
#include <cstdint>
#include <string>
#include <vector>

#include "raylib.h"


class MenuChoice {
public:
    MenuChoice(const char* menuTitle) :
        m_MenuTitle(menuTitle)
    {
        m_UpdateMenuString();
    };

public:
    void Draw(float scaleFactor);

public:
    void AddChoice(const char* text, const char* key);

private:
    struct MenuItem
    {
        MenuItem(const char* text, const char* keycode) :
            Text(text), KeyCode(keycode) {};

        std::string Text;
        std::string KeyCode;
    };

private:
    std::string m_MenuTitle = "";
    // all selectable menu items
    std::vector<MenuItem> m_MenuItems;
    // complete menu text as string for drawing
    std::string m_MenuDrawText = "";
    // screen positions of drawn elements
    std::vector<Vector2> m_MenuItemPositions;

private:
    void m_UpdateMenuString();

};



#endif //MENUCHOICE_H
