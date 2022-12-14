#include "FanController.hpp"
#include "Settings.h"
#include <cstdint>
#include <stdint.h>
#include "Utilities.hpp"

FanController::FanController(const PinName& tachometerPin_, const PinName& pwmOutputPin_)
    : tachometerPin(tachometerPin_)
    , pwmOutputPin(pwmOutputPin_)
    // set Main Thread with normal priority and 2048 bytes stack size
    , thread(FanControllerPriority, 2048, nullptr, "FanController") 
{
    // set high frequency PWM for fan control
    pwmOutputPin.period_us(20000);
    // attach callback function calcFanSpeed on falling edge (pin pulled up in hardware)
    tachometerPin.fall(callback(this, &FanController::tachometerISR));
}


void FanController::init()
{
    // start main thread
    thread.start(callback(this, &FanController::MainThread));
}

void FanController::deinit()
{
    // stops fan and terminates thread
    stopFan();
    thread.terminate();
}


void FanController::stopFan()
{
    desiredSpeed_RPM = 0;
    pwmOutputPin.write(0);
}


void FanController::setPWMOutFq(uint16_t frequency_Hz)
{ 
    uint16_t timePeriod_us = (1.0/frequency_Hz)*1e6;
    pwmOutputPin.period_us(timePeriod_us);
}


void FanController::MainThread()
{
    while(true)
    {
        // calculate new speed
        calculateCurrentSpeed();

        // // error is difference between desired speed and actual speed
        // float error = currentSpeed_RPM - desiredSpeed_RPM;
        
        // /* PWM Fan Output Calculated from PID */
        // static uint32_t previousTime = 0;
        // static uint32_t previousError = 0;
        // static float integralError = 0;

        // // calculate time elapsed since last call
        // uint64_t timeElapsed = mainTimer.elapsed_time().count();
        // // reset ISR timer
        // mainTimer.reset();
        // mainTimer.start();

        // // Do not calculate PID on first call since timeElapsed is undetermined
        // static bool firstCall = true;
        // if(!firstCall)
        // {
        //     firstCall = false;

        //     // calculate cumaltive error
        //     integralError += error * timeElapsed;

        //     // calculate change in error
        //     float derivativeError = (error - previousError)/timeElapsed;
        //     previousError = error;

        //     float PWM_Output = Settings::Fan::kp * (error + (Settings::Fan::ki * integralError) + (Settings::Fan::kd * derivativeError));

        //     if(PWM_Output > 1.0) PWM_Output = 1.0;
        //     else if (PWM_Output < 0.0) PWM_Output = 0.0;
        //     pwmOutputPin.write(PWM_Output);
        // }
        
        ThisThread::sleep_for(Settings::Fan::threadTimeInterval_ms);
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

    // ignore avg caluclation on first interrupt, since unknown pulse time
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
    // convert to RPM
    currentSpeed_RPM = 60 / (Settings::Fan::TachoPulsesPerRev * averagePulseTime_us * usToS);

    // reset ISR variables
    tachoCount = 0;
    averagePulseTime_us = 0;
}


void FanController::setDesiredSpeed_RPM(const uint16_t speed)
{
    desiredSpeed_RPM = speed;
    float pwmOut = static_cast<float>(speed)/Settings::Fan::MaxSpeed_RPM;
    printf("PWM OUT: %f\n", pwmOut);
    pwmOutputPin.write(pwmOut);
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