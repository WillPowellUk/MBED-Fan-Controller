/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "IMenu.hpp"
#include "Settings.h"
#include "Utilities.hpp"
#include <cstdint>

enum MenuType
{
    Brightness,
    Contrast,
    Difficulty,
    OpenLoop
};

class BarMenu : public IMenu
{
public:
    BarMenu(MenuType menu, const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu, uint16_t* updateTime_ms = nullptr)
        : IMenu(title, lcdBaseClass, parentMenu, nullptr)
        , menu(menu)
        , updateTime_ms(updateTime_ms)
    {
    }

    virtual void run() override
    {
        // print menu title
        lcdBase->lcd.printCentral(MenuTitle);

        // get current duty cylce (brightness) as a percentage and draw bar on lcd
        float currentValue_Percentage = getValue();
        lcdBase->lcd.printBar8UDC(1, currentValue_Percentage);

        while(true)
        {
            // if user rotates encoder, increment or decrement brightness
            if(lcdBase->encoder.getTics() != 0)
            {
                // increment or decrement bar (1 tic equals one pixel increment on bar)
                currentValue_Percentage += lcdBase->encoder.getTics() / static_cast<float>(lcdBase->lcd.pixelsWide());

                // ensure brightness is in set limits for minimum and maximum brightness
                if(currentValue_Percentage < 0.0) currentValue_Percentage = 0.0;
                else if (currentValue_Percentage > 1.0) currentValue_Percentage = 1.0;

                // set new brightness and update bar
                setValue(currentValue_Percentage);
                lcdBase->lcd.printBar8UDC(1, currentValue_Percentage);

                // reset tics to zero
                lcdBase->encoder.reset();
            }

            // check if button is pressed
            Button::state state = lcdBase->button.checkNewPresses();
            // return to previous menu (unless no parent menu i.e. Main Menu) on long press
            if((state == Button::state::Long_Press) && (parentMenu!= nullptr)) 
            {
                lcdBase->fan.setDesiredSpeed_Percentage(0.0);
                parentMenu->run();
            }
            
            // sleep to ensure other threads have time to run
            ThisThread::sleep_for(5ms);
        }
    }

private:
    MenuType menu;
    uint16_t* updateTime_ms;

    float getValue()
    {
        switch (menu) 
        {
            case MenuType::Brightness:
                return lcdBase->lcd.getBrightness();
            case MenuType::Contrast:
                return lcdBase->lcd.getContrast();
            case MenuType::Difficulty:
                return *updateTime_ms;
            case MenuType::OpenLoop:
                return 0.0;
        }
    }

    void setValue(float percentage)
    {
        switch (menu) 
        {
            case MenuType::Brightness:
            {
                lcdBase->lcd.setBrightness(percentage);
                break;
            }
            case MenuType::Contrast:
            {
                lcdBase->lcd.setContrast(percentage);
                break;
            }
            case MenuType::Difficulty:
            {
                setDifficulty(percentage);
                break;
            }
            case MenuType::OpenLoop:
            {
                lcdBase->fan.setDesiredSpeed_Percentage(percentage);
                break;
            }
        }
    }

    void setDifficulty(float percentage)
    {
        *updateTime_ms = static_cast<uint16_t>(Utilities::map(percentage, 0.0, 1.0, 1000, 100));
    }

};

