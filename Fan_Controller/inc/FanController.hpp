#pragma once
#include "mbed.h"
#include <cstdint>

#define usToS 1e-6

class FanController
{
public:
    /** Initialises the fan gpio pins and sets interrupt
    @param tachometerPin fan tachometer pin used as Interrupt input
    @param pwmOutputPin fan signal PWM Output
    */
    FanController(const PinName& tachometerPin_, const PinName& pwmOutputPin_);

    /** Initialises LCD and starts main thread
    */
    void init();

    /** Stops fan and terminates thread
    */
    void deinit();

    /** Main thread to control fan speed, using PID
    */
    void MainThread();

    /** Setter for desiredSpeed_RPM - target speed in RPM for control system
    @param speed speed in RPM
    */
    void setDesiredSpeed_RPM(const uint16_t speed);

    /** Setter for desiredSpeed_RPM - target speed as a percentage for control system
    @param speed speed as a percentage of max speed
    */
    void setDesiredSpeed_Percentage(const float speed);

    /** Getter for currentSpeed_RPM
    @return current speed of fan in RPM
    */
    uint16_t getCurrentSpeed_RPM() const;

        /** Calculates current speed in RPM based on tachometer's ISR
    */
    void calculateCurrentSpeed();

private:
    // hardware pins
    InterruptIn tachometerPin;
    PwmOut pwmOutputPin;

    // desired speed set by setDesiredSpeed
    uint16_t desiredSpeed_RPM = 0; 
    uint16_t currentSpeed_RPM = 0;

    // Main thread will run concurrently with other tasks
    Thread thread;
    // Timer for PID calculation
    Timer mainTimer;

    /** ISR Callback function from tachometerPin interrupt
    @details Updates counter until a set number have been triggered and then updates current speed
    */
    void tachometerISR();
    Timer ISRTimer;
    volatile uint32_t tachoCount = 0;
    volatile uint64_t averagePulseTime_us = 0;

};