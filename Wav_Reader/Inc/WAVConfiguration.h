/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "mbed.h"

namespace WAV
{
    #pragma pack(push, 1) // to prevent padding
    struct Header
    {
        char id[4];          // should always contain "RIFF"
        int totalLength;    // total file length minus 8
        char waveFormat[8];     // should be "WAVEfmt "
        int format;         // 16 for PCM format
        short pcm;            // 1 for PCM format
        short channels;       // channels
        int frequency;      // sampling frequency
        int bytesPerSecond;
        short bytesPerCapture;
        short bitsPerSample;
    };
    #pragma pack(pop)

    enum ReturnType
    {   
        Unable_To_Read_File,
        End_Of_File
    };

}

