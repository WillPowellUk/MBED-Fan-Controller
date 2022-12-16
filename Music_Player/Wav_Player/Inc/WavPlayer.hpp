/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "mbed.h"
#include "SDCardDriver.hpp"
#include "WAVConfiguration.h"

namespace WAV 
{

class Player : public SDCardDriver
{
public:

    Player();

    void play();

    void pause();

    void resume();

    void restart();

private:
    WAV::Header header;
    fpos_t filePosition;
    uint8_t currentAmplitude = 0;
};

}