#pragma once 


/** Closed loop methods that take in an error and returns an appropriate 
output as a percentage based on tuned constants*/
class ClosedLoopMethods
{
public:
    ClosedLoopMethods() = default;

    enum Method
    {
        PID,
        PI,
        PD,
        P
    };

    static float calcPID(float error, Method method);

    
};