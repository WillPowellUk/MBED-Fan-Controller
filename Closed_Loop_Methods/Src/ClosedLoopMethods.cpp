#include "ClosedLoopMethods.hpp"
#include "Settings.h"

float ClosedLoopMethods::calcPID(float error, Method method)
{
    // Do not calculate the integral or derivative on first call since timeElapsed is undetermined
    static bool firstCall = true;
    static Timer timer;

    static uint32_t previousTime = 0;
    static uint32_t previousError = 0;
    static float integralError = 0;

    // calculate time elapsed since last call
    uint64_t timeElapsed = timer.elapsed_time().count();
    // reset ISR timer
    timer.reset();
    timer.start();

    if(!firstCall)
    {
        // calculate cumaltive error
        integralError += error * timeElapsed;

        // calculate change in error
        float derivativeError = (error - previousError) / timeElapsed;
        previousError = error;

        float PWM_Output;

        switch (method) 
        {
            case Method::PID:
                PWM_Output = Settings::Fan::PID_kp * (error + (Settings::Fan::PID_ki * integralError) + (Settings::Fan::PID_kd * derivativeError));
                break;
            case Method::PI:
                PWM_Output = Settings::Fan::PI_kp * (error + (Settings::Fan::PI_ki * integralError));
                break;
            case Method::PD:
                PWM_Output = Settings::Fan::PD_kp * (error + (Settings::Fan::PID_kd * derivativeError));
                break;
            case Method::P:
                PWM_Output = Settings::Fan::PD_kp * error;
                break;
        }
        
        return PWM_Output;
    }
    else 
    {
        firstCall = false;
        return 0.0;
    }
    
}