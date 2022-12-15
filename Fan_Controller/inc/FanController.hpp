#pragma once
#include "mbed.h"
#include "Settings.h"
#include <chrono>
#include <cstdint>
#include "ClosedLoopMethods.hpp"

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

    /** Set Pwm frequency of output (useful for playing music)
    */
    void setPWMOutFrequency_Hz(uint16_t frequency_Hz);

    // Closed loop active method that runs in main thread (PID by default)
    ClosedLoopMethods::Method activeMethod = ClosedLoopMethods::Method::PID;

private:
    // hardware pins
    InterruptIn tachometerPin;
    PwmOut pwmOutputPin;

    // desired speed set by setDesiredSpeed
    uint16_t desiredSpeed_RPM = 0; 
    uint16_t currentSpeed_RPM = 0;

    // Tachometer pulse width extemeties (used in bandpass filter and pulse stretching)
    const uint32_t MinTachoPulseWidth_us = 60e6 / (Settings::Fan::MaxSpeed_RPM * Settings::Fan::TachoPulsesPerRev);
    const uint32_t MaxTachoPulseWidth_us = 60e6 / (Settings::Fan::MinSpeed_RPM * Settings::Fan::TachoPulsesPerRev);

    /* set PWM frequency higher than maximum tachometer frequency to ensure PWM pulse is not 
    mistaken as a tachometer reading during bandpass filtering */
    const uint32_t PWMOutPeriod_us = MinTachoPulseWidth_us - 500; // 500 us tolerance

    // Main thread will run concurrently with other tasks
    Thread mainThread;
    std::chrono::milliseconds mainThreadDelay = FanControlYieldTime;
    // Pulse stretching thread
    Thread pulseStretchingThread;

    // Two different ISR Callback options for the tachometerPin interrupt
    /** Updates counter until a set number have been triggered and then updates current speed
    */
    void bandpassFilterISR();
    void pulseStretchingISR();

    Timer ISRTimer;
    volatile uint32_t tachoCount = 0;
    volatile uint64_t averagePulseTime_us = 0;

    /** Stretches the pulse of the duty cycle temporarily to obtain accurate tachometer reading
    @details Ran constantly on pulseStretchingThread (blocking)
    */
    void pulseStretching();
    volatile bool pulseStretchingActive = false;
    volatile bool pulseStretchingComplete = false;

};