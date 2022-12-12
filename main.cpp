#include "mbed.h"

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

    // THREADS (does not include fan controller which is initiated through user input)
    lcdUI.init(); // configures lcd and starts main thread for UI
    button.init(); // starts main thread for button handling
    
    // main must be kept running (but not used)
    while(true) 
    {
        // printf("Running\n");
        ThisThread::sleep_for(500ms);
    }
}
