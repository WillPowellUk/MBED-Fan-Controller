#pragma once
#include "mbed.h"
#include <vector>
#include "LCDBaseClass.hpp"

// All the Menus
#include "IMenu.hpp"
#include "ParentMenu.hpp"
#include "BarMenu.hpp"
#include "ClosedLoopMenu.hpp"


/* This class handles the setup of Menu subclasses and starts the UI thread*/
class LCDUI
{
public:
    LCDUI(LCDBaseClass& lcdBase);

    /** Initialises LCD and starts main thread
    */
    void init();

    void setBrightnessMenu();
    void setContrastMenu();

private:
    LCDBaseClass& lcdBase;
    
    /* 
    Menu hierarchy descending from parent to child menus

                                    Main Menu
        Settings                    FanControl               Music Player
    Contrast, Brightness        OpenLoop, ClosedLoop        Track1, Track2...
    */

    // Parent Menus with child / submenus
    ParentMenu mainMenu;
    ParentMenu settingsMenu;
    ParentMenu fanControlMenu;
    ParentMenu closedLoopMenu;

    // child menus
    BarMenu brightnessMenu;
    BarMenu contrastMenu;
    BarMenu openLoopMenu;
    ClosedLoopMenu pidMenu;

    // child menus / sub menus for each parent menu      
    std::vector<IMenu*> mainMenuChilds;
    std::vector<IMenu*> settingsMenuChilds;
    std::vector<IMenu*> fanControlMenuChilds;
    std::vector<IMenu*> closedLoopMenuChilds;

    // Main thread will run concurrently with other tasks
    Thread thread;
};