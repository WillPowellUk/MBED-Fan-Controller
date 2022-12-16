/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "FlashMusic.h"
#include "LCDBaseClass.hpp"
#include <cstdint>
#include <stdint.h>

constexpr const int numOfTracks = 3;

class FlashPlayer
{
public:

    FlashPlayer(FanController& fan);

    // plays the music of trackNo from tracks
    void play_NonBlocking(uint16_t* trackNo, bool* pauseMusicFlag);

    static FlashMusic tracks[numOfTracks];

private:
    FanController& fan;

    // timer to prevent blocking wait for sampling frequency delay
    Timer samplingRateTimer;
};