/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#include "LCDBaseClass.hpp"

LCDBaseClass::LCDBaseClass(IEncoder& encoder, ILCDDriver& lcd, ButtonHandler& button, FanController& fan)
    : encoder(encoder)
    , lcd(lcd)
    , button(button)
    , fan(fan)
{

}



