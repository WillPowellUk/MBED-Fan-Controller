/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once 
#include <string>

namespace Utilities 
{
    inline double constrain(double x, double min_, double max_)
    {
        if (x > max_) return max_;
        if (x < min_) return min_;
        return x;
    }

    inline double map(double value, double in_min, double in_max, double out_min, double out_max) {
        return (constrain(value, in_min, in_max) - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    inline uint16_t random(uint16_t smallest, uint16_t largest)
    {
        return rand() % (largest-smallest) + smallest;
    }
}