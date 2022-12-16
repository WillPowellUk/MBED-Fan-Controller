/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include <stdint.h>
#include <stddef.h>

struct FlashMusic
{
    // title of track
    const char* Title;
    // sampling rate of track
    uint16_t samplingFq;
    // mono channel spectrogram
    uint16_t spectrogram[20];
};