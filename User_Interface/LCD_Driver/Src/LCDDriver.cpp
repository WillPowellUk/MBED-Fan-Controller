#include "LCDDriver.hpp"
#include "mbed.h"
// #include <cstdint>
// #include <cstring>
#include "Utilities.hpp"
#include <string.h>

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


void LCDDriver::printRight(const char* text, int row)
{
    // meausre text length to find column index
    locate((lcd.columns() - strlen(text)), row);
    lcd.printf("%s", text);
}

void LCDDriver::printBar16UDC(uint8_t row, float percentage)
{
    // increments through entire row, and writes pixel high or low to make bar
    char customChar[rowResolution];

    // Set limit for column number to be filled up to
    const uint16_t pixelColLimit = percentage * numOfPixelsPerRow;

    // increment through characters
    for(int character = 0; character<lcd.columns(); character++)
    {
        // nested incrementation through each row in each character
        for (int characterRow = 0; characterRow<rowResolution; characterRow++)
        {
            uint8_t rowPixels = 0;

            // nested incrementation through each pixel in each row in each character
            for(int pixelIndex = 0; pixelIndex<colResolution; pixelIndex++)
            {
                // pixel value by default is low (off)
                bool pixelVal = 0;

                // Set high up to pixel column limit to fill bar up to percentage
                const uint16_t colPixelIndex = (character * colResolution) + pixelIndex;
                if (colPixelIndex <= pixelColLimit) pixelVal = true; 

                // Or set high if on bar perimeter
                else if (characterRow == 0) pixelVal = true; 
                else if (characterRow == (rowResolution-1)) pixelVal = true; 
                else if ((character == 0) && (pixelIndex == 0)) pixelVal = true; 
                else if ((character == (lcd.columns()-1)) && (pixelIndex == (colResolution-1))) pixelVal = true; 
                
                // add pixelValue to rowPixels using bitshifting
                rowPixels = rowPixels | (pixelVal << pixelIndex);                
            }
            // add row to customChar array
            customChar[characterRow] = rowPixels;
        }
        // print each custom char
        lcd.locate(character, row);
        lcd.setUDC(0, customChar);
        lcd.putc(0);
    }
}


void LCDDriver::printBar8UDC(uint8_t row, float percentage)
{
    // set custom characters
    lcd.setUDC(0, const_cast<char*>(udc_fifth_full));
    lcd.setUDC(1, const_cast<char*>(udc_two_fifths_full));
    lcd.setUDC(2, const_cast<char*>(udc_three_fifths_full));
    lcd.setUDC(3, const_cast<char*>(udc_four_fifths_full));
    lcd.setUDC(4, const_cast<char*>(udc_fill));
    lcd.setUDC(5, const_cast<char*>(udc_left_perimeter));
    lcd.setUDC(6, const_cast<char*>(udc_top_and_bottom_perimeter));
    lcd.setUDC(7, const_cast<char*>(udc_right_perimeter));

    // calculate number of pixels to fill until
    uint8_t pixelFillIndex = percentage * numOfPixelsPerRow; 

    // increment through characters
    for(int col = 0; col<lcd.columns(); col++)
    {
        // set cursor to current column
        lcd.locate(col, row);

        // caclulate difference between limit and the current number of pixels
        int16_t pixelDifference = pixelFillIndex - (col * colResolution);
        // if difference is greater or equal to column resolution, print full character
        if (pixelDifference >= colResolution) lcd.putc(4); 
        // otherwise if positive difference, print bar proportional to that amount
        else if (pixelDifference > 0) lcd.putc(pixelDifference); 
        // otherwise if negative pixel distance, print perimeter only
        else 
        {
            // left perimeter
            if(col == 0) lcd.putc(5);
            // central perimeter
            else if (col == (lcd.columns()-1)) lcd.putc(7);
            // right perimeter
            else lcd.putc(6);
        }
    }
}

void LCDDriver::setBrightness(float percentage)
{
    // map to a percentage of min/max values and write to PWM 
    float value = Utilities::map(percentage, 0, 1, Settings::LCD::minBrightness, Settings::LCD::maxBrightness);
    backlightPin.write(value);
}


void LCDDriver::setContrast(float percentage)
{
    // map to a percentage of min/max values and write to PWM 
    float value = Utilities::map(percentage, 0, 1, Settings::LCD::minContrast, Settings::LCD::maxContrast);
    backlightPin.write(value);
}


float LCDDriver::getBrightness()
{
    // map to a percentage of min/max values
    return Utilities::map(backlightPin.read(), Settings::LCD::minContrast, Settings::LCD::maxContrast, 0, 1);
}

float LCDDriver::getContrast()
{
    // map to a percentage of min/max values
    return Utilities::map(contrastPin.read(), Settings::LCD::minContrast, Settings::LCD::maxContrast, 0, 1);
}

uint16_t LCDDriver::pixelsWide()
{
    return lcd.columns() * colResolution;
}

uint16_t LCDDriver::pixelsTall()
{
    return lcd.rows() * rowResolution;
}


/* Wrapper Methods */
void LCDDriver::setUDC(unsigned char c, char *udc_data)
{
    lcd.setUDC(c, (char*)udc_data);
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