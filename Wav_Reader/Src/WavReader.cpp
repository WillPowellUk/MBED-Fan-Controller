/*
* WAV_Reader.cpp
*
* Created on: 29 October 2022
*     Author: William Powell
*/

#include "WAVReader.hpp"
#include <cstdio>
#include <stdio.h>
#include "mbed_events.h"

WAV::Reader::Reader()
    : SDCardDriver(SD_MOSI, SD_MISO, SD_SCK, SD_CS)
{

}

void WAV::Reader::play()
{
    FILE* file = fopen(activeFileName,"rb");
    if (file == NULL) 
    {
        status = Settings::Play_Failed;
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


void WAV::Reader::pause()
{
    // store the current position in the file
    // fgetpos(activeFile, &filePosition);
}

