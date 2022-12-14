#pragma once 
#include "mbed.h"
#include <cstdint>

/* GPIO Hardware Pins */
// Button
#define BuiltInButton PC_13 

// Encoder Pins
#define Encoder_Ch_A PA_1
#define Encoder_Ch_B PA_4

// Fan Pins
#define Fan_Tachometer_Pin PA_0
#define FAN_PWM_Out_Pin PB_0

// SD Card SPI Pins
#define SD_MOSI D11
#define SD_MISO D12
#define SD_SCK D13
#define SD_CS D10

// LED 
#define LED_BIDIR_A PB_7
#define LED_BIDIR_B PA_15
#define LED PC_0

// LCD Display Pins
#define LCD_RS PB_15
#define LCD_E PB_14
#define LCD_DB0 PA_10
#define LCD_DB1 PB_3
#define LCD_DB2 PB_5
#define LCD_DB3 PB_4
#define LCD_DB4 PB_10
#define LCD_DB5 PA_8
#define LCD_DB6 PB_2
#define LCD_DB7 PB_1
#define LCD_Backlight PB_6
#define LCD_Contrast PC_7

// FAN PWM 
#define PWM_0 D9

// required for printing thread stats
#define SerialPrint

// thread priority levels
#define ButtonHandlerPriority osPriorityRealtime
#define LCDUIPriority osPriorityAboveNormal
#define FanControllerPriority osPriorityNormal
#define FlashPlayerPriority osPriorityBelowNormal

namespace Settings 
{
    namespace Fan
    {
        /* HARDWARE PARAMETERS */
        // number of tachometer pulses per revolution of the fan
        const constexpr uint8_t TachoPulsesPerRev = 4;
        // Maximum speed of fan in RPM
        const constexpr uint16_t MaxSpeed_RPM = 2300;
        // Minimum pwm required to start rotating the fan
        const constexpr float minPWMOut = 0.01000;

        /*  TUNING PARAMETERS */

        // interval time for main thread - updating fan speed and sending new PID values
        // More accurate fan speed with longer interval, however control system is slower to respond to error
        const constexpr uint16_t threadTimeInterval_ms = 100;
        // PID constants
        const constexpr float kp = 1.0;
        const constexpr float ki = 1.0;
        const constexpr float kd = 1.0;

    }

    namespace Encoder
    {
        // Number of encodings per revolution (resolution of encoder)
        const constexpr uint16_t encodingsPerRev = 12;

        // Number of encodings per mechanical tic
        const constexpr uint16_t encodingsPerMechanicalTic = 4;

        /* Maximum time in microseconds when a rising or falling edge should be ignored to debounce encoder. */
        const constexpr uint32_t callbackTimeInterval_us = 1;
    }

    namespace LCD
    {
        struct PinConfiguration
        {
            PinName rs = LCD_RS;
            PinName e = LCD_E;
            PinName d0 = LCD_DB0;
            PinName d1 = LCD_DB1;
            PinName d2 = LCD_DB2;
            PinName d3 = LCD_DB3;
            PinName d4 = LCD_DB4;
            PinName d5 = LCD_DB5;
            PinName d6 = LCD_DB6;
            PinName d7 = LCD_DB7;
            PinName contrast = LCD_Contrast;
            PinName backlight = LCD_Backlight;
        };
        const constexpr float defaultBrightness = 0.5f;
        const constexpr float maxBrightness = 1.0f;
        const constexpr float minBrightness = 0.3f;

        const constexpr float defaultContrast = 0.5f;
        const constexpr float maxContrast = 1.0f;
        const constexpr float minContrast = 0.3f;

        const constexpr uint64_t screenRefreshRate_us = 500e3;
    }

    namespace SD
    {
        enum Status
        {
            OK,
            SD_Init_Failed,
            File_Does_Not_Exist,
            Write_Failed,
            Play_Failed
        };
    }

    namespace BuiltInButton
    {
        constexpr const bool pulledHigh = true;
    }
}
