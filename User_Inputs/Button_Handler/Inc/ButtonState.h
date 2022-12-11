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