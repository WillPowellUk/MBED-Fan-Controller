#pragma once
#include <stdint.h>
#include <stddef.h>

struct FlashMusic
{
    const char* Title;
    uint16_t samplingFq;
    uint16_t frequencies[100];
};