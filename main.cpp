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

#include "FlashPlayer.hpp"

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

    // THREADS (does not include fan controller or music player threads which are initiated during user input)
    lcdUI.init(); // configures lcd and starts main thread for UI
    button.init(); // starts main thread for button handling

    // main must be kept running (but not used)
    while(true) 
    {
        // if(encoder.getMechanicalTics() > 0)
        // {
        //     frequency += (encoder.getMechanicalTics() * 10);
        //     encoder.reset();
        //     fan.setPWMOutFq(frequency);
        //     lcd.cls();
        //     lcd.printf("Frequency: %u", frequency);
        // }


        // mbed_stats_thread_t* stats = new mbed_stats_thread_t[10];
        // int count = mbed_stats_thread_get_each(stats, 10);
        // for (int i = 0; i < count; i++) {
        //     printf("ID: 0x%" PRIx32 "\n", stats[i].id);
        //     printf("Name: %s \n", stats[i].name);
        //     printf("State: %" PRId32 "\n", stats[i].state);
        //     printf("Priority: %" PRId32 "\n", stats[i].priority);
        //     printf("Stack Size: %" PRId32 "\n", stats[i].stack_size);
        //     printf("Stack Space: %" PRId32 "\n", stats[i].stack_space);
        //     printf("\n");
        // }

        // delete[] stats;

        // printf("Running\n");
        ThisThread::sleep_for(500ms);
    }
}
