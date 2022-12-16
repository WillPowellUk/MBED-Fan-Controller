#pragma once
#include "FlashMusic.h"
#include "LCDBaseClass.hpp"
#include <cstdint>
#include <stdint.h>

constexpr const int numOfTracks = 0;

class FlashPlayer
{
public:

    FlashPlayer(FanController& fan);

    // plays the music of trackNo from tracks
    void play_NonBlocking(uint16_t* trackNo, bool* pauseMusicFlag);

    static FlashMusic tracks[numOfTracks];

private:
    FanController& fan;
};