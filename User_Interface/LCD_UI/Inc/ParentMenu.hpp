#pragma once
#include "IMenu.hpp"
#include <vector>


class ParentMenu : public IMenu
{
public:
    /** Constructor
    @param Title takes in title of Menu
    */
    ParentMenu(const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu, std::vector<IMenu*>* childMenus)
        : IMenu(title, lcdBaseClass, parentMenu, childMenus)
    {
    }

    virtual void run() override
    {
        // print title and first child menu title
        uint16_t childIndex = 0;
        lcdBase->lcd.printCentral(MenuTitle);
        lcdBase->lcd.locate(0,1);
        lcdBase->lcd.printf("%s", childMenus->at(childIndex)->MenuTitle);

        /* wait for user to select menu */
        // reset encoder tics to zero
        lcdBase->encoder.reset();

        while(true)
        {
            // if user rotates encoder, scroll through childMenus
            if((lcdBase->encoder.getMechanicalTics()) != 0)
            {
                // increment or decrement through menus (infinite scrolling)
                if (lcdBase->encoder.getMechanicalTics() > 0) childIndex = (childIndex + 1) % childMenus->size();
                else childIndex = (childIndex - 1) % childMenus->size();
                // reset tics to zero
                lcdBase->encoder.reset();
                
                // print title and submenu titles
                lcdBase->lcd.printCentral(MenuTitle);
                lcdBase->lcd.locate(0,1);
                lcdBase->lcd.printf("%s", childMenus->at(childIndex)->MenuTitle);
            }
            // check if button is pressed
            Button::state state = lcdBase->button.checkNewPresses();
            // select active child menu on short press
            if (state == Button::state::Short_Press) 
            {
                lcdBase->lcd.printCentral("Banter");
                childMenus->at(childIndex)->run();
            }
            // return to previous menu (unless no parent menu i.e. Main Menu) on long press
            else if((state == Button::state::Long_Press) && (parentMenu!= nullptr)) 
            {
                parentMenu->run();
            }
            
            // sleep to ensure other threads have time to run
            ThisThread::sleep_for(10ms);
        }
    }
};


