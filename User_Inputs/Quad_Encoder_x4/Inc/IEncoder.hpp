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