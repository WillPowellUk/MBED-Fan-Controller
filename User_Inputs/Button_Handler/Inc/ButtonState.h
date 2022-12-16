/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#pragma once

namespace Button
{
    const constexpr auto longPressTime_us = 500000;

    enum state
    {
        No_New_Press,
        Short_Press,
        Long_Press
    };
}