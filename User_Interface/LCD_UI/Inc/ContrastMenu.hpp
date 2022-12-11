#pragma once
#include "IMenu.hpp"


class ContrastMenu : public IMenu
{
public:
    ContrastMenu(const char* MainTitle, LCDBaseClass* lcdBaseClass, IMenu* parentMenu)
        : IMenu(MainTitle, lcdBaseClass, parentMenu, nullptr)
    {
    }

    virtual void run() override
    {
        lcdBase->lcd.printCentral(MenuTitle);
    }
};