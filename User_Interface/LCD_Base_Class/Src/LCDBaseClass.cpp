#include "LCDBaseClass.hpp"

LCDBaseClass::LCDBaseClass(IEncoder& encoder, ILCDDriver& lcd, ButtonHandler& button, FanController& fan)
    : encoder(encoder)
    , lcd(lcd)
    , button(button)
    , fan(fan)
{

}



