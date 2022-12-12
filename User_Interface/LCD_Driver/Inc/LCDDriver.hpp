#pragma once 
#include "ILCDDriver.hpp"
#include "TextLCD.hpp"
#include "Settings.h"
#include "mbed.h"
#include <cstdint>


/* This class uses the Enhanced TextLCD driver and implements additional features*/
class LCDDriver : public ILCDDriver
{
public:
    LCDDriver(Settings::LCD::PinConfiguration& pinConfig);
    
    virtual void printCentral(const char* text, int row=0) override;
    virtual void printRight(const char* text, int row=0) override;
    virtual void setUDC(unsigned char c, char *udc_data) override;
    virtual void locate(int column, int row) override;
    virtual void cls() override;

    virtual void setBrightness(float percentage) override;
    virtual void setContrast(float percentage) override;
    virtual float getBrightness() override;
    virtual float getContrast() override;

    /** Prints a bar stretching entirety of row (designed for 16 UDC compatible boards)
    @param col column to print to
    @param percentage bar percentage to fill
    */
    virtual void printBar16UDC(uint8_t colNum, float percentage) override;

    /** Prints a bar stretching entirety of row (designed for 8 UDC compatible boards)
    @param col column to print to
    @param percentage bar percentage to fill
    */
    virtual void printBar8UDC(uint8_t colNum, float percentage) override;

    // returns number of pixels wide / tall
    virtual uint16_t pixelsWide() override;
    virtual uint16_t pixelsTall() override;

private:
    // pins for backlight and contrast
    PwmOut backlightPin;
    PwmOut contrastPin;

    // Text LCD driver object
    TextLCD lcd;

    // Defintions of character resolution
    const uint8_t rowResolution = 8;
    const uint8_t colResolution = 5;
    const uint16_t numOfPixelsPerRow = lcd.columns() * colResolution;

    // Stream implementation functions
    virtual int _putc(int value) override;
    virtual int _getc() override;

};