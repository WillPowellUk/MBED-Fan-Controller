#pragma once
#include "IMenu.hpp"
#include "Settings.h"
#include "Utilities.hpp"
#include <chrono>
#include <cstdint>
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

        // initialise fan controller and pulse stretching thread
        lcdBase->fan.init();

        // print title, desired and actual speed
        lcdBase->lcd.printCentral(MenuTitle);
        lcdBase->lcd.locate(0,1);
        lcdBase->lcd.printf("%uRPM", lcdBase->fan.getCurrentSpeed_RPM());

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
            ThisThread::sleep_for(5);
        }
    }

private:
    ClosedLoopMethod method;
    Timer screenRefreshTimer;
};