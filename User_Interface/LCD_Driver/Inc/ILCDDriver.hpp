#pragma once
#include "mbed.h"
#include "stdint.h"


// This interface class serves as a template for any LCD Driver
class ILCDDriver : public Stream
{
public:
    // Print methods
    // Printf method inherited from Stream
    virtual void printCentral(const char* text, int row=0) = 0;
    virtual void printRight(const char* text, int row=0) = 0;
    virtual void setUDC(unsigned char c, char *udc_data) = 0;
    virtual void locate(int column, int row) = 0;
    virtual void cls() = 0;

    // Brightness and Contrast Control
    virtual void setBrightness(float percentage) = 0;
    virtual void setContrast(float percentage) = 0;
    virtual float getBrightness() = 0;
    virtual float getContrast() = 0;

    // get lcd display dimensions
    virtual uint16_t pixelsWide() = 0;
    virtual uint16_t pixelsTall() = 0;

    // Animations and custom prints
    virtual void printBar16UDC(uint8_t col, float percentage) = 0;
    virtual void printBar8UDC(uint8_t col, float percentage) = 0;

};