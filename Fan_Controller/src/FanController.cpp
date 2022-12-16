/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


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
    , mainThread(FanControllerPriority, 4096, nullptr, "FanController") 
{
    /* set PWM frequency higher than maximum tachometer frequency to ensure PWM pulse is not 
    mistaken as a tachometer reading during bandpass filtering */
    pwmOutputPin.period_us(PWMOutPeriod_us);

    // attach ISR to falling edge of tacho signal
    tachometerPin.fall(callback(this, &FanController::tachometerISR));

    // start main thread
    mainThread.start(callback(this, &FanController::MainThread));
    
}


void FanController::MainThread()
{
    // calculate delays in advance to save computational time
    // twice the time period to ensure at least one complete tachometer pulse is measured
    const uint32_t activeDelay_ms = (2 * MaxTachoPulseWidth_us) / 1e3; 
    const uint32_t inactiveDelay_ms = activeDelay_ms * Settings::Fan::PulseStretchRatio;

    while(true)
    {
        // check event flag
        uint32_t flag = closedLoopEvent.wait_any((ClosedLoopFlag | OpenLoopFlag), 0, false);

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
                pulseStretchingEvent.clear();
                pulseStretchingEvent.set(PulseStretchingActiveFlag);
                ThisThread::sleep_for(std::chrono::milliseconds(activeDelay_ms));

                // Go back to previous duty cycle for x pulses
                pulseStretchingEvent.clear();
                pulseStretchingEvent.set(PulseStretchingInactiveFlag);
                pwmOutputPin.write(currentDutyCycle);
            }
            ThisThread::sleep_for(std::chrono::milliseconds(inactiveDelay_ms));

            // calculate latest speed measurement 
            calculateCurrentSpeed();

            currentSpeedMutex.lock();
            desiredSpeedMutex.lock();
            // error is the difference between desired speed and actual speed relative to max speed
            float error = (static_cast<float>(desiredSpeed_RPM) - currentSpeed_RPM) / Settings::Fan::MaxSpeed_RPM;
            currentSpeedMutex.unlock();
            desiredSpeedMutex.unlock();

            // Add correction to feedback PWM
            activeMethodMutex.lock();
            float PWMFeedback = ClosedLoopMethods::calcPID(error, activeMethod);
            activeMethodMutex.unlock();
            
            // Check limits and output feedback to fan
            if(PWMFeedback > 1.0) PWMFeedback = 1.0;
            else if (PWMFeedback < 0.0) PWMFeedback = 0.0;
            pwmOutputPin.write(PWMFeedback);
        }
        else 
        {
            ThisThread::sleep_for(200ms);
        }
    }
}


void FanController::tachometerISR()
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
    uint32_t flag = pulseStretchingEvent.wait_any((PulseStretchingActiveFlag | PulseStretchingInactiveFlag), 0, false);
    if (flag != PulseStretchingActiveFlag) return;

    /* Bandpass Filter attenuates the tachometer frequencies that are not in range of the minimum and maximum frequencies, including PWM artifacts */
    if((pulseTime_us > (MinTachoPulseWidth_us)) && (pulseTime_us < (MaxTachoPulseWidth_us)))
    {
        // ignore avg caluclation on first interrupt, since pulse time is unknown
        if (tachoCount != 0)
        {
            // weight latest pulse time with previous average pulse times
            averagePulseTime_us = ((averagePulseTime_us * (tachoCount-1)) + pulseTime_us) / tachoCount;
        }
    }
    // increment tachometer counter
    tachoCount++;
    
}


void FanController::calculateCurrentSpeed()
{
    currentSpeedMutex.lock();
    // convert average time of one pulse to a speed in RPM 
    currentSpeed_RPM = 60 / (Settings::Fan::TachoPulsesPerRev * averagePulseTime_us * usToS);
    currentSpeedMutex.unlock();

    // reset ISR variables
    tachoCount = 0;
    averagePulseTime_us = 0;
}


void FanController::setDesiredSpeed_RPM(const uint16_t speed)
{
    desiredSpeedMutex.lock();
    desiredSpeed_RPM = speed;
    desiredSpeedMutex.unlock();
}


void FanController::setDesiredSpeed_Percentage(const float speed)
{
    desiredSpeedMutex.lock();
    desiredSpeed_RPM = speed * Settings::Fan::MaxSpeed_RPM;
    desiredSpeedMutex.unlock();
    pwmOutputPin.write(Utilities::map(speed, 0.0, 1.0, Settings::Fan::minPWMOut, 1.0));
}


void FanController::setActiveMethod(ClosedLoopMethods::Method method)
{
    activeMethodMutex.lock();
    activeMethod = method;
    activeMethodMutex.unlock();
}

uint16_t FanController::getCurrentSpeed_RPM()
{
    currentSpeedMutex.lock();
    uint16_t speed = currentSpeed_RPM;
    currentSpeedMutex.unlock();
    return speed;
}


void FanController::setPWMOutFrequency_Hz(uint16_t frequency_Hz)
{
    uint16_t timePeriod_us = (1.0/frequency_Hz)*1e6;
    if ((timePeriod_us > 50) && (timePeriod_us < 2e5))
        pwmOutputPin.period_us(timePeriod_us);
}