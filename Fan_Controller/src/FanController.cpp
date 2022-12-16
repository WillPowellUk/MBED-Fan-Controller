#include "FanController.hpp"
#include "Settings.h"
#include <chrono>
#include <cstdint>
#include <stdint.h>
#include "Utilities.hpp"

FanController::FanController(const PinName& tachometerPin_, const PinName& pwmOutputPin_)
    : tachometerPin(tachometerPin_)
    , pwmOutputPin(pwmOutputPin_)
    // set Main Thread with normal priority and 2048 bytes stack size
    , mainThread(FanControllerPriority, 2048, nullptr, "FanController") 
    , pulseStretchingThread(PulseStretchingPriority, 1024, nullptr, "PulseStretching")
{
    /* set PWM frequency higher than maximum tachometer frequency to ensure PWM pulse is not 
    mistaken as a tachometer reading during bandpass filtering */
    pwmOutputPin.period_us(PWMOutPeriod_us);

    // set tachometer ISR to bandpass or pulse stretching
    if(Settings::Fan::PulseStretchingActive)
    {
        // attach ISR to falling edge of tacho signal
        tachometerPin.fall(callback(this, &FanController::pulseStretchingISR));
    }
    else 
    {
        // attach ISR to both rising and falling edge of tacho signal
        tachometerPin.rise(callback(this, &FanController::bandpassFilterISR));
        tachometerPin.fall(callback(this, &FanController::bandpassFilterISR));
    }

    // start main thread and pulse stretching thread (if required)
    mainThread.start(callback(this, &FanController::MainThread));
    //if(Settings::Fan::PulseStretchingActive)
      //  pulseStretchingThread.start(callback(this, &FanController::pulseStretching));
    
}


void FanController::init()
{
}


void FanController::deinit()
{
    // stops fan and terminates thread
    setDesiredSpeed_Percentage(0);
    // mainThread.terminate();
}


void FanController::MainThread()
{
    // calculate delays in advance to save computational time
    // twice the time period to ensure at least one complete tachometer pulse is measured
    const uint32_t activeDelay_ms = (2 * MaxTachoPulseWidth_us) / 1e3; 
    const uint32_t inactiveDelay_ms = activeDelay_ms * Settings::Fan::PulseStretchRatio;

    while(true)
    {
        uint32_t flag = 0;
        // check event flag
        flag = closedLoopEvent.wait_any((ClosedLoopFlag | OpenLoopFlag), 0, false);

        // if in closed loop mode perform pulse stretching
        if(flag == ClosedLoopFlag)
        {
            // set to default PWM frequency
            pwmOutputPin.period_ms(20);

            // every x tachometer pulses, determined by PulsesPerPulseStretch, set duty cycle to 100% for one tachometer pulse width
            // do not conduct pulse stretching if desired fan speed is zero
            if(desiredSpeed_RPM != 0)
            {
                // store current duty cycle
                float currentDutyCycle = pwmOutputPin.read();

                // set duty cycle to 100% for one duty cycle and enable tachometer reading
                pwmOutputPin.write(1.0);
                pulseStretchingActive.clear();
                pulseStretchingActive.set(PulseStretchingActiveFlag);
                ThisThread::sleep_for(std::chrono::milliseconds(activeDelay_ms));

                // Go back to previous duty cycle for x pulses
                pulseStretchingActive.clear();
                pulseStretchingActive.set(PulseStretchingInactiveFlag);
                pwmOutputPin.write(currentDutyCycle);
            }

            ThisThread::sleep_for(std::chrono::milliseconds(inactiveDelay_ms));

            // calculate latest speed measurement 
            calculateCurrentSpeed();

            // error is the difference between desired speed and actual speed relative to max speed
            float error = (static_cast<float>(desiredSpeed_RPM) - currentSpeed_RPM) / Settings::Fan::MaxSpeed_RPM;

            // Add correction to feedback PWM
            float PWMFeedback = ClosedLoopMethods::calcPID(error, ClosedLoopMethods::Method::PID);
            
            // Check limits and output feedback to fan
            if(PWMFeedback > 1.0) PWMFeedback = 1.0;
            else if (PWMFeedback < 0.0) PWMFeedback = 0.0;
            pwmOutputPin.write(PWMFeedback);
        }
        else 
        {
            ThisThread::sleep_for(100ms);
        }
    }        
}


void FanController::bandpassFilterISR()
{
    /* In a given window, set by how often the method variables tachoCount and averagePulseTime_us 
    are reset (when calculateCurrentSpeed is called), the average time between pulses will be calculated */
    
    // calculate latest pulse time (between rising and falling edge)
    uint64_t pulseTime_us = ISRTimer.elapsed_time().count();
    
    // reset ISR timer
    ISRTimer.reset();
    ISRTimer.start();

    /* Bandpass Filter attenuates the tachometer frequencies that are not in range of the minimum and maximum frequencies, including PWM artifacts */
    // Pulse width is divided by two to account for both the rising and falling edge of a single pulse
    if((pulseTime_us > (MinTachoPulseWidth_us / 2)) && (pulseTime_us < (MaxTachoPulseWidth_us / 2)))
    {
        // ignore avg caluclation on first interrupt, since pulse time is unknown
        if (tachoCount != 0)
        {
            // weight latest pulse time with previous average pulse times
            averagePulseTime_us = ((averagePulseTime_us * (tachoCount-1)) + pulseTime_us) / tachoCount;
        }
        // increment tachometer counter
        tachoCount++;
    }
}


void FanController::pulseStretchingISR()
{
    /* In a given window, set by how often the method variables tachoCount and averagePulseTime_us 
    are reset (when calculateCurrentSpeed is called), the average time between pulses will be calculated */
    
    // calculate latest pulse time
    uint64_t pulseTime_us = ISRTimer.elapsed_time().count();
    
    // reset ISR timer
    ISRTimer.reset();
    ISRTimer.start();

    /* Pulse stretching ensures tachometer does not lose power and give false reading during pwm OFF cycle, 
    by outputting an ocassional 100% duty cycle pulse*/
    // Only take measurement if pulse stretching is active
    uint32_t flag = pulseStretchingActive.wait_any((PulseStretchingActiveFlag | PulseStretchingInactiveFlag), 0, false);
    if (flag != PulseStretchingActiveFlag) return;

    // ignore avg caluclation on first interrupt, since pulse time is unknown
    if (tachoCount != 0)
    {
        // weight latest pulse time with previous average pulse times
        averagePulseTime_us = ((averagePulseTime_us * (tachoCount-1)) + pulseTime_us) / tachoCount;
    }
    // increment tachometer counter
    tachoCount++;
    
}


void FanController::calculateCurrentSpeed()
{
    if (Settings::Fan::PulseStretchingActive)
        // convert time of one pulse to a speed in RPM 
        currentSpeed_RPM = 60 / (Settings::Fan::TachoPulsesPerRev * averagePulseTime_us * usToS);
    else 
        // convert time between rising and falling edge of one pulse to a speed in RPM 
        currentSpeed_RPM = 60 / (Settings::Fan::TachoPulsesPerRev * 2 * averagePulseTime_us * usToS);

    // reset ISR variables
    tachoCount = 0;
    averagePulseTime_us = 0;
}


void FanController::setDesiredSpeed_RPM(const uint16_t speed)
{
    desiredSpeed_RPM = speed;
}


void FanController::setDesiredSpeed_Percentage(const float speed)
{
    desiredSpeed_RPM = speed * Settings::Fan::MaxSpeed_RPM;
    pwmOutputPin.write(Utilities::map(speed, 0.0, 1.0, Settings::Fan::minPWMOut, 1.0));
}


uint16_t FanController::getCurrentSpeed_RPM() const
{
    return currentSpeed_RPM;
}


void FanController::setPWMOutFrequency_Hz(uint16_t frequency_Hz)
{
    uint16_t timePeriod_us = (1.0/frequency_Hz)*1e6;
    pwmOutputPin.period_us(timePeriod_us);
}