#pragma once
#include "mbed.h"
#include <vector>
#include "LCDBaseClass.hpp"

// All the Menus
#include "IMenu.hpp"
#include "GenericMenu.hpp"
#include "BrightnessMenu.hpp"
#include "ContrastMenu.hpp"



/* This class handles the setup of Menu subclasses and starts the UI thread*/
class LCDUI
{
public:
    LCDUI(LCDBaseClass& lcdBase);

    /** Initialises LCD and starts main thread
    */
    void init();

    /** Main thread displaying current menu
    */
    void MainThread();

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

    // Menus (with number of child classes)
    GenericMenu mainMenu;
    GenericMenu settingsMenu;
    BrightnessMenu brightnessMenu;
    ContrastMenu contrastMenu;

    // child menus (sub menus)        
    std::vector<IMenu*> mainMenuChilds;
    std::vector<IMenu*> settingsMenuChilds;

    // 
    void selectMenu();

    // Main thread will run concurrently with other tasks
    Thread thread;
};