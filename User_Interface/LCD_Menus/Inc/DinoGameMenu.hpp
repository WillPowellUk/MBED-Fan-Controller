#pragma once
#include "IMenu.hpp"
#include "DinoGame.hpp"
#include <cstdint>

class DinoGameMenu : public IMenu
{
public:
    DinoGameMenu(const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu)
        : IMenu(title, lcdBaseClass, parentMenu, nullptr)
        , dinoGame(lcdBaseClass->lcd, lcdBaseClass->button, &updateTime_ms)
    {
    }

    virtual void run() override
    {
        dinoGame.run();
        // if game is exited, go to parent menu
        parentMenu->run();
    }

    // Dino game difficulty paramater
    uint16_t updateTime_ms = 500;

private:
    DinoGame dinoGame;


};