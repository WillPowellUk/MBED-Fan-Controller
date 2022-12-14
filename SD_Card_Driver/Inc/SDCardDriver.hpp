/*
* SD_Card_Driver.hpp
*
* Created on: 29 October 2022
*     Author: William Powell
*/

#pragma once
#include <mbed.h>
#include "SDBlockDevice.h"
#include "FATFileSystem.h"
#include "Settings.h"

class SDCardDriver
{
public:
    /** Mounts sd card and checks it can be read properly
    */
    SDCardDriver(const PinName& MOSI, const PinName& MISO, const PinName& SCK, const PinName& CS);

    // initialises sd card
    void init();

    /** Sets active file 
    @param path takes path to file
    */
    void setActiveFile(const char* path);

    /** Reads active file as ASCI characters
    */
    void readActiveFile(); 

    /** Writes to active file (overwrites previous file if it exists)
    @param text you would like to write to fill
    */
    void writeActiveFile(const char* text);

    /** Un-mounts sd card
    */
    void deinit();

    /** Getter for paramater activeFileName
    @return string of file name
    */
    const char* getActiveFileName();

    // Current status displaying 
    Settings::SD::Status status = Settings::SD::Status::OK;

protected:
    SDBlockDevice sd;
    FATFileSystem fs;
    char* activeFileName;
};