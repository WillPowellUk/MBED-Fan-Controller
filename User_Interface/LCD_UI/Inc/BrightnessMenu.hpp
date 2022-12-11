#pragma once
#include "IMenu.hpp"


class BrightnessMenu : public IMenu
{
public:
    BrightnessMenu(const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu)
        : IMenu(title, lcdBaseClass, parentMenu, nullptr)
    {
    }

    virtual void run() override
    {
        lcdBase->lcd.printCentral(MenuTitle);
    }
};