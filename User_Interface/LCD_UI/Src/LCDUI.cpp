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
    , closedLoopMenu("Closed Loop", &lcdBase, &fanControlMenu, &closedLoopMenuChilds)
    , pidMenu(ClosedLoopMethod::PID, "PID", &lcdBase, &closedLoopMenu)
    , brightnessMenu(MenuType::Brightness, "Brightness", &lcdBase, &settingsMenu)
    , contrastMenu(MenuType::Contrast, "Contrast", &lcdBase, &settingsMenu)
    , openLoopMenu(MenuType::OpenLoop, "Open Loop", &lcdBase, &fanControlMenu)
{
    mainMenuChilds = {&fanControlMenu, &settingsMenu};
    settingsMenuChilds = {&brightnessMenu, &contrastMenu};
    fanControlMenuChilds = {&closedLoopMenu, &openLoopMenu};
    closedLoopMenuChilds = {&pidMenu};
}


void LCDUI::init()
{
    lcdBase.lcd.setBrightness(Settings::LCD::defaultBrightness);
    lcdBase.lcd.setContrast(Settings::LCD::defaultContrast);

    // run main menu method (blocking)
    mainMenu.run();
}