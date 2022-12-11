#include "LCDUI.hpp"
#include "Settings.h"
#include <cstdint>
#include <iterator>

LCDUI::LCDUI(LCDBaseClass& lcdBase)
    : lcdBase(lcdBase)
    // initialise all the menu objects
    , mainMenu("Main Menu", &lcdBase, nullptr, &mainMenuChilds)
    , settingsMenu("Settings", &lcdBase, &mainMenu, &settingsMenuChilds)
    , brightnessMenu("Brightness", &lcdBase, &settingsMenu)
    , contrastMenu("Contrast", &lcdBase, &settingsMenu)
    // set Main Thread with relatively high priority and 4096 bytes stack size
    , thread(osPriorityAboveNormal, 4096, nullptr, "LCDUI") 
{
    mainMenuChilds = {&settingsMenu};
    settingsMenuChilds = {&brightnessMenu, &contrastMenu};
}


void LCDUI::init()
{
    //lcdBase.lcd.setBrightness(Settings::LCD::defaultBrightness);
    //lcdBase.lcd.setContrast(Settings::LCD::defaultContrast);

    thread.start(callback(this, &LCDUI::MainThread));
}


void LCDUI::MainThread()
{
    while(true)
    {
        // run main menu method
        mainMenu.run();

        // sleep this task to allow other tasks to run
        ThisThread::sleep_for(10ms);
    }
}