#include "ButtonHandler.hpp"


ButtonHandler::ButtonHandler(const PinName& pin_, bool pulledHigh)
    : pin(pin_)
    , pulledHigh(pulledHigh)
    // set Main Thread with high priority and 2048 bytes stack size
    , thread(osPriorityRealtime, 2048, nullptr, "ButtonHandler") 
{
    // set button to trigge on rising or falling edge
    if (pulledHigh) pin.fall(callback(this, &ButtonHandler::buttonISR));
    else pin.rise(callback(this, &ButtonHandler::buttonISR));
}


void ButtonHandler::init()
{
    thread.start(callback(this, &ButtonHandler::MainThread));
}


Button::state ButtonHandler::checkNewPresses()
{
    /* check queue and return first (oldest) from the queue, 
    if nothing in queue returns no new press */
    Button::state* state;
    if(buttonStateQueue.try_get(&state))
    {
        // copy state as need to delete from heap
        Button::state stateCpy = *state;
        delete state;
        return stateCpy;
    }
    return Button::state::No_New_Press;
}


void ButtonHandler::buttonISR()
{
    // create new boolean on heap
    bool* buttonPressed = new bool;
    *(buttonPressed) = true;
    MBED_ASSERT(ISRButtonQueue.try_put(buttonPressed));
}


void ButtonHandler::MainThread()
{
    while(true)
    {
        // checks queue and passes to newPress 
        bool* newPress;
        if(ISRButtonQueue.try_get(&newPress))
        {
            // delete heap memory to prevent memory leak
            delete newPress;

            // start timer
            Timer buttonTimer;
            buttonTimer.reset();
            buttonTimer.start();

            // while button is still pressed, record time that button has been pressed for
            while(true)
            {
                // measure time taken
                uint32_t timePressed_us = buttonTimer.elapsed_time().count();
    
                // if long press time has been reached, add longPress to queue and exit
                if(timePressed_us > Button::longPressTime_us)
                {
                    Button::state* state = new Button::state;
                    *(state) = Button::state::Long_Press;  
                    MBED_ASSERT(buttonStateQueue.try_put(state));
                    break;
                }
                // if button press has been released before longPressTime, add shortPress to queue and exit
                if(pin.read() == pulledHigh)
                {
                    Button::state* state = new Button::state;
                    *(state) = Button::state::Short_Press;  
                    MBED_ASSERT(buttonStateQueue.try_put(state));
                    break;
                }
                ThisThread::sleep_for(10ms);
            }
        }
        ThisThread::sleep_for(10ms);
    }
}