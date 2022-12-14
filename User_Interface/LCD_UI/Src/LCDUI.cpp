#include "LCDUI.hpp"
#include "Settings.h"
#include <cstdint>
#include <iterator>

LCDUI::LCDUI(LCDBaseClass& lcdBase)
    : lcdBase(lcdBase)
    // initialise all the menu objects
    , mainMenu("Main Menu", &lcdBase, nullptr, &mainMenuChilds)
    , settingsMenu("Settings", &lcdBase, &mainMenu, &settingsMenuChilds)
    , fanControlMenu("Fan Control", &lcdBase, &mainMenu, &fanControlMenuChilds)
    , musicMenu("Music Player", &lcdBase, &mainMenu)
    , closedLoopMenu("Closed Loop", &lcdBase, &fanControlMenu, &closedLoopMenuChilds)
    , pidMenu(ClosedLoopMethod::PID, "PID", &lcdBase, &closedLoopMenu)
    , brightnessMenu(MenuType::Brightness, "Brightness", &lcdBase, &settingsMenu)
    , contrastMenu(MenuType::Contrast, "Contrast", &lcdBase, &settingsMenu)
    , openLoopMenu(MenuType::OpenLoop, "Open Loop", &lcdBase, &fanControlMenu)
    // set Main Thread with relatively high priority and 4096 bytes stack size
    , thread(LCDUIPriority, 4096, nullptr, "LCDUI") 
{
    mainMenuChilds = {&fanControlMenu, &musicMenu, &settingsMenu};
    settingsMenuChilds = {&brightnessMenu, &contrastMenu};
    fanControlMenuChilds = {&closedLoopMenu, &openLoopMenu};
    closedLoopMenuChilds = {&pidMenu};
}


void LCDUI::init()
{
    lcdBase.lcd.setBrightness(Settings::LCD::defaultBrightness);
    lcdBase.lcd.setContrast(Settings::LCD::defaultContrast);

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