#pragma once
#include "FlashMusic.h"
#include "LCDBaseClass.hpp"
#include <cstdint>
#include <stdint.h>

constexpr const int numOfTracks = 2;

class FlashPlayer
{
public:
    // create tuple for function input arguments
    struct ThreadInputArgs
    {
        volatile uint16_t trackNo;
        volatile bool pauseFlag;
    };

    FlashPlayer(FanController& fan);

    // plays the music of trackNo from tracks
    void play();

    // starts thread
    void init();
    // terminates thread
    void deinit();

    static FlashMusic tracks[numOfTracks];
    static ThreadInputArgs args;

private:
    FanController& fan;
    Thread thread;
};