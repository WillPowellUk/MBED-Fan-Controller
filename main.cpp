/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/

#include "mbed.h"
#include <inttypes.h>

// Settings
#include "Settings.h"

// High Level Classes
#include "LCDUI.hpp"
#include "LCDBaseClass.hpp"

// Drivers
#include "LCDDriver.hpp"
#include "QuadEncoderX4.hpp"
#include "ButtonHandler.hpp"
#include <cstdint>


int main() 
{
    // create pin configuration instance for LCD
    Settings::LCD::PinConfiguration pinConfig;

    // initiates lcd, encoder, button and fan drivers
    LCDDriver lcd(pinConfig);
    QuadEncoderX4 encoder(Encoder_Ch_A, Encoder_Ch_B);
    ButtonHandler button(BuiltInButton, Settings::BuiltInButton::pulledHigh);
    FanController fan(Fan_Tachometer_Pin, FAN_PWM_Out_Pin);

    // passes all objects to LCD base class
    LCDBaseClass lcdBase(encoder, lcd, button, fan);

    // main LCDUI object that initialises child menus with LCDBaseClass
    LCDUI lcdUI(lcdBase);

    // THREADS (does not include fan controller which is initiated during user input)
    button.init(); // starts button handling thread
    lcdUI.init(); // configures lcd and starts UI (on main thread)
}
