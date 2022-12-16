/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "IEncoder.hpp"
#include "ILCDDriver.hpp"
#include "ButtonHandler.hpp"
#include "FanController.hpp"

class LCDBaseClass
{
public:
    LCDBaseClass(IEncoder& encoder, ILCDDriver& lcd, ButtonHandler& button, FanController& fan);
    IEncoder& encoder;
    ILCDDriver& lcd;
    ButtonHandler& button;
    FanController& fan;
};