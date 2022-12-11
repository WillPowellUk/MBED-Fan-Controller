#include "LCDDriver.hpp"
#include "mbed.h"
#include <cstring>

LCDDriver::LCDDriver(Settings::LCD::PinConfiguration& pinConfig)
    : lcd(pinConfig)
    , backlightPin(pinConfig.backlight)
    , contrastPin(pinConfig.contrast)

{
    // set high frequency pwm for backlight and contrast pins to prevent flickering
    backlightPin.period_us(200);
    contrastPin.period_us(200);
    
    // set default brightness
    backlightPin.write(Settings::LCD::defaultBrightness);
    contrastPin.write(Settings::LCD::defaultContrast);
}

void LCDDriver::printCentral(const char *text, int row)
{
    // clears lcd
    lcd.cls();

    // meausre text length to find column index
    int columnIndex = (lcd.columns() - strlen(text)) / 2;
    locate(columnIndex, row);
    lcd.printf("%s", text);
}


void LCDDriver::setBrightness(float value)
{
    backlightPin.write(value);
}


void LCDDriver::setContrast(float value)
{
    contrastPin.write(value);
}



/* Wrapper Methods */
void LCDDriver::setUDC(char* udc_data)
{
    lcd.setUDC(0, (char*)udc_data);
}


void LCDDriver::locate(int column, int row)
{
    lcd.locate(column, row);
}

void LCDDriver::cls()
{
    lcd.cls();
}


int LCDDriver::_putc(int value)
{
    int returnVal = lcd._putc(value);
    return returnVal;
}

int LCDDriver::_getc()
{
    return -1;
}