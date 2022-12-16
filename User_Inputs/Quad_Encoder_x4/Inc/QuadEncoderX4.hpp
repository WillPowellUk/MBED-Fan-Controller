/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once
#include "mbed.h"
#include "IEncoder.hpp"
/*
 * For maximum resolution of the quadrature encoder, this library uses X4 encoding to calculate pulse count 
 * by reading the current state after each rising and falling edge of either channel.

 *
 *               +-----+     +-----+     +-----+
 * Channel A     |     |     |     |     |     |
 *            ---+     +-----+     +-----+     +-----
 *               ^     ^     ^     ^     ^
 *               ^  +-----+  ^  +-----+  ^  +-----+
 * Channel B     ^  |  ^  |  ^  |  ^  |  ^  |     |
 *            ------+  ^  +-----+  ^  +-----+     +--
 *               ^  ^  ^  ^  ^  ^  ^  ^  ^  ^
 *               ^  ^  ^  ^  ^  ^  ^  ^  ^  ^
 * Tic count     0 1  2  3  4  5  6  7  8  9  ...

    The number of revolutions is determined by finding the tic count / number of encodings per revolution
*/

class QuadEncoderX4 : public IEncoder
{
public:
    /** Initialises the encoder by setting interrupt pins etc.
    @param ChannelA 
    @param ChannelB
    */
    QuadEncoderX4(const PinName& ChannelA, const PinName& ChannelB);

    /* Resets count and revolutions of motor
    */
    virtual void reset() override;

    /** Getter for tic count
    @return the current tic count of the encoder, positive is clockwise, negative is anticlockwise
    */
    virtual int16_t getTics() const override;

    /** Getter for mechanical tic count
    @return the current tic count of the encoder, positive is clockwise, negative is anticlockwise
    */
    virtual int16_t getMechanicalTics() const override;

    /** Getter for revolutions
    @return the current revolutions of the encoder, positive is clockwise, negative is anticlockwise
    */
    virtual int16_t getRevolutions() const override;


private:
    /** Callback function on interrupt of Channel A and Channel B
    */
    void encode_ISR();

    // previous state of encoder using last two significant bits for channelA and channelB respectively.
    volatile uint8_t previousState;
    volatile uint8_t currentState;
    volatile int16_t ticCount = 0;
    Timer ISRTimer;

    InterruptIn channelA;
    InterruptIn channelB;

};