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
#include <chrono>
#include <cstdint>
#include <stdio.h>
#include "ClosedLoopMethods.hpp"

class ClosedLoopMenu : public IMenu
{
public:
    ClosedLoopMenu(ClosedLoopMethods::Method method, const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu)
        : IMenu(title, lcdBaseClass, parentMenu, nullptr)
        , method(method)
    {
    }

    virtual void run() override
    {   
        // set Closed Loop Method
        lcdBase->fan.setActiveMethod(method);
        
        // set fan speed to zero
        int16_t desiredSpeedRPM = 0;
        lcdBase->fan.setDesiredSpeed_RPM(desiredSpeedRPM);
        // reset encoder
        lcdBase->encoder.reset();

        // set closed loop flag
        lcdBase->fan.closedLoopEvent.clear();
        lcdBase->fan.closedLoopEvent.set(ClosedLoopFlag);

        // print title, desired and actual speed
        lcdBase->lcd.printCentral(MenuTitle);
        lcdBase->lcd.locate(0,1);
        lcdBase->lcd.printf("%uRPM", 0);

        char actualSpeedStr[7];
        std::sprintf(actualSpeedStr, "%uRPM", desiredSpeedRPM);
        lcdBase->lcd.printRight(actualSpeedStr, 1);

        // start screen refresh timer
        screenRefreshTimer.reset();
        screenRefreshTimer.start();

        while(true)
        {
            // refresh screen periodically
            if(screenRefreshTimer.elapsed_time() > std::chrono::microseconds(Settings::LCD::screenRefreshRate_us))
            {
                // reset timer
                screenRefreshTimer.reset();
                screenRefreshTimer.start();

                // print title, desired and actual speed
                lcdBase->lcd.printCentral(MenuTitle);
                lcdBase->lcd.locate(0,1);
                lcdBase->lcd.printf("%uRPM", desiredSpeedRPM);
                std::sprintf(actualSpeedStr, "%uRPM", lcdBase->fan.getCurrentSpeed_RPM());
                lcdBase->lcd.printRight(actualSpeedStr, 1);
            }

            // if user rotates encoder, increment or decrement fan speed
            if(lcdBase->encoder.getMechanicalTics() != 0)
            {
                // first increment starts at minimum speed
                if((desiredSpeedRPM == 0) && (lcdBase->encoder.getMechanicalTics() > 0))
                    desiredSpeedRPM = Settings::Fan::MinSpeed_RPM;
                else if((desiredSpeedRPM == Settings::Fan::MinSpeed_RPM) && (lcdBase->encoder.getMechanicalTics() < 0))
                    desiredSpeedRPM = 0;

                // Otherwise one mech tic corresponds to 10 RPM
                else
                    desiredSpeedRPM += (lcdBase->encoder.getMechanicalTics() * 10);

                // reset encoder
                lcdBase->encoder.reset();

                // limit it to viable range and set
                if (desiredSpeedRPM > Settings::Fan::MaxSpeed_RPM) desiredSpeedRPM = Settings::Fan::MaxSpeed_RPM;
                else if (desiredSpeedRPM < 0) desiredSpeedRPM = 0;
                lcdBase->fan.setDesiredSpeed_RPM(desiredSpeedRPM);

                // yield to other thread if necessary
                ThisThread::yield();

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
                // set back to open loop flag
                lcdBase->fan.closedLoopEvent.clear();
                lcdBase->fan.closedLoopEvent.set(OpenLoopFlag);
                lcdBase->fan.setDesiredSpeed_RPM(0);
                parentMenu->run();
            }
            // allow other tasks to run
            ThisThread::sleep_for(std::chrono::milliseconds(LCDUIYieldTime));
        }
    }

private:
    ClosedLoopMethods::Method method;
    Timer screenRefreshTimer;
};