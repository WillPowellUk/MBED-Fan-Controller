#include "QuadEncoderX4.hpp"
#include <cstdint>
#include <iterator>
#include "Settings.h"

QuadEncoderX4::QuadEncoderX4(const PinName& ChannelA, const PinName& ChannelB)
    : channelA(ChannelA)
    , channelB(ChannelB)
{
    // set callbacks for rising and falling interrupts on both channels
    channelA.rise(callback(this, &QuadEncoderX4::encode_ISR));
    channelA.fall(callback(this, &QuadEncoderX4::encode_ISR));
    channelB.rise(callback(this, &QuadEncoderX4::encode_ISR));
    channelB.fall(callback(this, &QuadEncoderX4::encode_ISR));

    // get current state of the pins and set as previous state to prepare for next encode callback
    previousState = (channelA.read() << 1) | (channelB.read());

}


void QuadEncoderX4::encode_ISR()
{
    // calculate latest pulse time
    uint64_t lastCallbackTime_us = ISRTimer.elapsed_time().count();
    
    // reset ISR timer
    ISRTimer.reset();
    ISRTimer.start();

    // Do not increment if callback is triggered before callbackTimeInterval
    if(lastCallbackTime_us < Settings::Encoder::callbackTimeInterval_us) return;

    // get current state using last two significant bits for channelA and channelB respectively.
    currentState = (channelA.read() << 1) | (channelB.read());

    // Ignore invalid states - if either both bits have changed (since encoder follows Gray Encoding) or niether bits have changed since last callback.
    // this is done by XORing current and previous state.
    if (((currentState ^ previousState) != 0x3) && (currentState != previousState)) 
    {
        /* To determine direction: 

        For Clockwise:
            Channel:    A   B
                        1   1
                        0   1
                        0   0
                        1   0

        For Anti-clockwise:
            Channel:    A   B
                        1   1
                        1   0
                        0   0
                        0   1

        Therefore, if you XOR Channel B of previous state  with Channel A of current state, 
        the result will be 1 for clockwise and 0 for anticlockwise
        */
        bool result = (previousState & 0x1) ^ ((currentState & 0x2)>>1);

        // increment +1 for clockwise, -1 for anticlockwise
        (result) ? ticCount++ : ticCount--;
    }

    // update previous state
    previousState = currentState;
}


void QuadEncoderX4::reset()
{
    ticCount = 0;
}

int16_t QuadEncoderX4::getTics() const 
{
    return ticCount;
}

int16_t QuadEncoderX4::getMechanicalTics() const
{
    return ticCount/Settings::Encoder::encodingsPerMechanicalTic;
}

int16_t QuadEncoderX4::getRevolutions() const
{
    return ticCount/Settings::Encoder::encodingsPerRev;
}