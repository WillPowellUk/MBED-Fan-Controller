/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "stdint.h"

class IEncoder
{
public:
    virtual int16_t getTics() const = 0;
    virtual int16_t getMechanicalTics() const = 0;
    virtual int16_t getRevolutions() const = 0;
    virtual void reset() = 0;
};