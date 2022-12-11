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