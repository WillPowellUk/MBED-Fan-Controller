/*
* WAV_Reader.hpp
*
* Created on: 29 October 2022
*     Author: William Powell
*/

#pragma once
#include "mbed.h"
#include "SDCardDriver.hpp"
#include "WAVConfiguration.h"

namespace WAV 
{

class Reader : public SDCardDriver
{
public:

    Reader();

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