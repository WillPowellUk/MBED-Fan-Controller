#pragma once
#include "mbed.h"
#include "stdint.h"


// This interface class serves as a template for any LCD Driver
class ILCDDriver : public Stream
{
public:

    /* int printf(const char* format, ...) inherited from Stream*/

    virtual void printCentral(const char* text, int row=0) = 0;

    virtual void setUDC(char* udc_data) = 0;

    virtual void locate(int column, int row) = 0;
 
    virtual void cls() = 0;

    virtual void setBrightness(float value) = 0;
    virtual void setContrast(float value) = 0;

};