/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/

#pragma once
#include "mbed.h"
#include "rtos.h"
#include "ButtonState.h"

class ButtonHandler
{
public:
    /** 
    @param pin the button pin that an interrupt will be attached to
    */
    ButtonHandler(const PinName& pin_, bool pulledHigh=false);

    /** Starts main thread
    */
    void init();

    /** Returns latest button press
    @return the button state - (No_New_Press, Short_Press, Long_Press)
    */
    Button::state checkNewPresses();

private:
    /** ISR callback for button
    @details adds new button press to queue for processing
    */
    void buttonISR();

    /** Main thread that processes new button presses
    @details 
    */
    void MainThread();

    // Main thread to handle button presses
    Thread thread;

    InterruptIn pin;
    bool pulledHigh = false;

    // Queue for handling button press from ISR
    Queue<bool, 32> ISRButtonQueue;
    // Queue for processing button press in Main thread
    Queue<Button::state, 32> buttonStateQueue;

};