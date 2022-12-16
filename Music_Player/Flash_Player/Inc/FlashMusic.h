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
    uint16_t spectrogram[100];
};