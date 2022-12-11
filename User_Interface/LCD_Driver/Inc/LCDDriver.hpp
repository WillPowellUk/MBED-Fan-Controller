#pragma once 
#include "ILCDDriver.hpp"
#include "TextLCD.hpp"
#include "Settings.h"
#include "mbed.h"


/* This class uses the Enhanced TextLCD driver and implements additional features*/
class LCDDriver : public ILCDDriver
{
public:
    LCDDriver(Settings::LCD::PinConfiguration& pinConfig);
    
    virtual void printCentral(const char* text, int row=0) override;
    virtual void setUDC(char* udc_data) override;
    virtual void locate(int column, int row) override;
    virtual void cls() override;

    virtual void setBrightness(float value) override;
    virtual void setContrast(float value) override;

private:
    // Text LCD 
    TextLCD lcd;

    PwmOut backlightPin;
    PwmOut contrastPin;

    // Stream implementation functions
    virtual int _putc(int value) override;
    virtual int _getc() override;

};