/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/

#include "WavPlayer.hpp"
#include <cstdio>
#include <stdio.h>
#include "mbed_events.h"

WAV::Player::Player()
    : SDCardDriver(SD_MOSI, SD_MISO, SD_SCK, SD_CS)
{

}

void WAV::Player::play()
{
    FILE* file = fopen(activeFileName,"rb");
    if (file == NULL) 
    {
        status = Settings::SD::Status::Play_Failed;
        return;
    }

    // read the contents of WAV File Header 
    fread(&header, sizeof(header), 1, file);

    // print details of WAV File Header if required
    #ifdef SerialPrint
    printf("----- WAV CONTENTS -----\n");
    printf("ID: ");
    for( int i = 0; i < sizeof(header.id); i ++)
        printf("%c", header.id[i]);
    printf("\n");
    printf("Total Length: %i\n", header.totalLength);
    printf("Wave Format: %s", header.waveFormat);
    printf("Format: %i\n", header.format);
    printf("PCM: %i\n", header.pcm);
    printf("Channels: %i\n", header.channels);
    printf("Frequency: %i\n", header.frequency);
    printf("Bytes Per Second: %i\n", header.bytesPerSecond);
    printf("Bits Per Sample: %i\n\n", header.bitsPerSample);
    #endif

    fclose(file);
}


void WAV::Player::pause()
{
    // store the current position in the file
    // fgetpos(activeFile, &filePosition);
}

