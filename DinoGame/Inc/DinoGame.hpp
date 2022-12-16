/*  Author: CMULAY
    https://github.com/cmulay/Arduino-UNO-Dinosaur-Game
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "mbed.h"
#include "ILCDDriver.hpp"
#include "ButtonHandler.hpp"
#include <chrono>
#include <cstdint>

class DinoGame
{
public:
    DinoGame(ILCDDriver& lcd, ButtonHandler& button, uint16_t* updateTime_ms);

    void run();

private:
    ILCDDriver& lcd;
    ButtonHandler& button;

    bool playState = false;
    bool exitGame = false;
    
    uint16_t* updateTime_ms;

    void startGame();
};