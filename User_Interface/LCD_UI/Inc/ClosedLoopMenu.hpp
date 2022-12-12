#pragma once
#include "IMenu.hpp"
#include "Settings.h"
#include "Utilities.hpp"
#include <stdio.h>

enum ClosedLoopMethod
{
    PID,
    PI,
    PD
};


class ClosedLoopMenu : public IMenu
{
public:
    ClosedLoopMenu(ClosedLoopMethod method, const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu)
        : IMenu(title, lcdBaseClass, parentMenu, nullptr)
        , method(method)
    {
    }

    virtual void run() override
    {
        // set fan speed to zero
        uint16_t desiredSpeedRPM = 0;
        lcdBase->fan.setDesiredSpeed_RPM(desiredSpeedRPM);

        // initialise fan controller thread
        lcdBase->fan.init();

        // print title, desired and actual speed
        lcdBase->lcd.printCentral(MenuTitle);
        lcdBase->lcd.locate(0,1);
        lcdBase->lcd.printf("%uRPM", lcdBase->fan.getCurrentSpeed_RPM());

        char actualSpeedStr[7];
        std::sprintf(actualSpeedStr, "%uRPM", lcdBase->fan.getCurrentSpeed_RPM());
        lcdBase->lcd.printRight(actualSpeedStr, 1);

        while(true)
        {
            // if user rotates encoder, increment or decrement fan speed
            if(lcdBase->encoder.getTics() != 0)
            {
                // one tic corresponds to one RPM
                desiredSpeedRPM += lcdBase->encoder.getTics();
                lcdBase->encoder.reset();
                lcdBase->fan.setDesiredSpeed_RPM(desiredSpeedRPM);

                // print title, desired and actual speed
                lcdBase->lcd.printCentral(MenuTitle);
                lcdBase->lcd.locate(0,1);
                lcdBase->lcd.printf("%uRPM", desiredSpeedRPM);
                std::sprintf(actualSpeedStr, "%uRPM", lcdBase->fan.getCurrentSpeed_RPM());
                lcdBase->lcd.printRight(actualSpeedStr, 1);
            }
            // check if button is pressed
            Button::state state = lcdBase->button.checkNewPresses();
            // return to previous menu (unless no parent menu i.e. Main Menu) on long press
            if((state == Button::state::Long_Press) && (parentMenu!= nullptr))
            {
                // stop fan and kill thread before exiting menu
                lcdBase->fan.deinit();
                parentMenu->run();
            }
            // allow other tasks to run
            ThisThread::sleep_for(5ms);
        }
    }

private:
    ClosedLoopMethod method;
};