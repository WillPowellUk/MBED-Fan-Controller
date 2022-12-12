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
}