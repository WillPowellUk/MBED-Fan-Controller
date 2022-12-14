/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/

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
    , dinoGameMenu("Dino Game", &lcdBase, &mainMenu)
    , musicMenu("Music Player", &lcdBase, &mainMenu)
    , closedLoopMenu("Closed Loop", &lcdBase, &fanControlMenu, &closedLoopMenuChilds)
    , pidMenu(ClosedLoopMethods::Method::PID, "PID", &lcdBase, &closedLoopMenu)
    , pdMenu(ClosedLoopMethods::Method::PD, "PD", &lcdBase, &closedLoopMenu)
    , piMenu(ClosedLoopMethods::Method::PI, "PI", &lcdBase, &closedLoopMenu)
    , pMenu(ClosedLoopMethods::Method::PID, "P", &lcdBase, &closedLoopMenu)
    , brightnessMenu(MenuType::Brightness, "Brightness", &lcdBase, &settingsMenu)
    , contrastMenu(MenuType::Contrast, "Contrast", &lcdBase, &settingsMenu)
    , gameDifficultyMenu(MenuType::Difficulty, "Game Difficulty", &lcdBase, &settingsMenu, &dinoGameMenu.updateTime_ms)
    , openLoopMenu(MenuType::OpenLoop, "Open Loop", &lcdBase, &fanControlMenu)
{
    mainMenuChilds = {&fanControlMenu, &musicMenu, &dinoGameMenu, &settingsMenu};
    settingsMenuChilds = {&brightnessMenu, &contrastMenu};
    fanControlMenuChilds = {&closedLoopMenu, &openLoopMenu};
    closedLoopMenuChilds = {&pidMenu}; // &pdMenu, &piMenu, &pMenu};
}


void LCDUI::init()
{
    lcdBase.lcd.setBrightness(Settings::LCD::defaultBrightness);
    lcdBase.lcd.setContrast(Settings::LCD::defaultContrast);

    // run main menu method (blocking)
    mainMenu.run();
}