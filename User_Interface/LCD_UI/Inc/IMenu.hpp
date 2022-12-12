#pragma once 
#include "LCDBaseClass.hpp"
#include <vector>

// This is an interface class that all Menu option classes will inherit
class IMenu
{
public:
    // Main method to select child menus accordingly (can be overwritten)
    virtual void run() = 0;
    // Title of Menu
    const char* MenuTitle;

    // lcd base class with lcd, encoder and button drivers
    LCDBaseClass* lcdBase;

    // pointer to parent's menu class
    IMenu* parentMenu;

    // vector of pointers to child menu classes
    std::vector<IMenu*>* childMenus;

    IMenu(){}

protected:
    IMenu(const char* MenuTitle, LCDBaseClass* lcdBaseClass, IMenu* parentMenu, std::vector<IMenu*>* childMenus = nullptr)
        : MenuTitle(MenuTitle)
        , lcdBase(lcdBaseClass)
        , parentMenu(parentMenu)
        , childMenus(childMenus)
    {
    }

};