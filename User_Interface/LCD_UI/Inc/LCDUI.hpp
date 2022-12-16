/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "mbed.h"
#include <cstdint>
#include <vector>
#include "LCDBaseClass.hpp"

// All the Menus
#include "IMenu.hpp"
#include "ParentMenu.hpp"
#include "BarMenu.hpp"
#include "ClosedLoopMenu.hpp"
#include "MusicPlayerMenu.hpp"
#include "DinoGameMenu.hpp"

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
    MusicPlayerMenu musicMenu;
    DinoGameMenu dinoGameMenu;
    BarMenu brightnessMenu;
    BarMenu contrastMenu;
    BarMenu gameDifficultyMenu;
    BarMenu openLoopMenu;
    ClosedLoopMenu pidMenu;
    ClosedLoopMenu pdMenu;
    ClosedLoopMenu piMenu;
    ClosedLoopMenu pMenu;

    // child menus / sub menus for each parent menu      
    std::vector<IMenu*> mainMenuChilds;
    std::vector<IMenu*> settingsMenuChilds;
    std::vector<IMenu*> fanControlMenuChilds;
    std::vector<IMenu*> closedLoopMenuChilds;

    // Main thread will run concurrently with other tasks
    Thread thread;
};