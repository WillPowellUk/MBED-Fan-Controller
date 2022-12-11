/*
* SD_Card_Driver.cpp
*
*  Created on: 29 October 2022
*      Author: William Powell
*/

#include "SDCardDriver.hpp"
#include "Settings.h"
#include "mbed.h"

SDCardDriver::SDCardDriver(const PinName& MOSI, const PinName& MISO, const PinName& SCK, const PinName& CS)
    : sd(MOSI, MISO, SCK, CS)
    , fs("sd", &sd)
{
    if (sd.init() != 0) status = Settings::Status::SD_Init_Failed;
}

void SDCardDriver::setActiveFile(const char* path)
{
    /* if memory has previously been allocated to activeFileName, free it to prevent memory leakage */
    if(activeFileName != NULL) free(activeFileName);

    // concatenate the path to rootpath and set as activeFileName 
    const char* rootPath = "/sd/";
    activeFileName = static_cast<char*>(malloc(1+strlen(rootPath) + strlen(path)));
    strcpy(activeFileName, rootPath);
    strcat(activeFileName, path);
}

void SDCardDriver::readActiveFile()
{
    FILE* file = fopen(activeFileName, "r");
    if (file == NULL) 
    {
        status = Settings::Status::File_Does_Not_Exist;
        return;
    }

    int c = getc(file);
    // read till end of file
    while (c != EOF) 
    {   
        // to get cursor back into original position of stream
        ungetc(c, file);
        printf("%c", getc(file));
        c = getc(file);
    }
    printf("\n");
    fclose(file);
}

void SDCardDriver::writeActiveFile(const char* text)
{
    printf("Writing To File\n");
    FILE* file = fopen(activeFileName,"w");
    if (file == NULL) 
    {
        status = Settings::Status::Write_Failed;
        return;
    }
    fprintf(file, text);
    fclose(file);
}

void SDCardDriver::deinit()
{
    sd.deinit();
}

const char* SDCardDriver::getActiveFileName()
{
    return activeFileName;
}