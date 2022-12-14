#pragma once
#include "IMenu.hpp"
#include "SDCardDriver.hpp"
#include "FlashPlayer.hpp"
#include "TextLCD_UDC.h"
#include "Settings.h"
#include <cstdint>
#include <tuple>

enum symbol
{
    leftTrack,
    play,
    rightTrack,
    leftTrackInv,
    pause,
    pauseInv,
    playInv,
    rightTrackInv,
};

class MusicPlayerMenu : public IMenu
{
public:
    MusicPlayerMenu(const char* title, LCDBaseClass* lcdBaseClass, IMenu* parentMenu)
        : IMenu(title, lcdBaseClass, parentMenu, nullptr)
        , SD(SD_MOSI, SD_MISO, SD_SCK, SD_CS)
        , flashPlayer(lcdBase->fan)
    {
    }

    virtual void run() override
    {
        // Initialise sd card
        SD.init();

        // check if sd card is initialised correctly
        if(SD.status == Settings::SD::SD_Init_Failed)
        {
            lcdBase->lcd.printCentral("SD Card Init Failed!");
            ThisThread::sleep_for(500ms);
            lcdBase->lcd.locate(0,1);
            lcdBase->lcd.printf("Loading Flash Music Tracks");  
            ThisThread::sleep_for(500ms);
        }

        // print title, and music tracks
        flashPlayer.args.trackNo = 0;
        lcdBase->lcd.printCentral(MenuTitle);
        lcdBase->lcd.locate(0,1);
        lcdBase->lcd.printf("%s", flashPlayer.tracks[0].Title);

        // reset encoder tics to zero
        lcdBase->encoder.reset();

        /* wait for user to select track */
        while(true)
        {
            // if user rotates encoder, scroll through tracks
            if((lcdBase->encoder.getMechanicalTics()) != 0)
            {
                // increment or decrement through menus (infinite scrolling)
                if (lcdBase->encoder.getMechanicalTics() > 0) flashPlayer.args.trackNo = (flashPlayer.args.trackNo + 1) % numOfTracks;
                else flashPlayer.args.trackNo = (flashPlayer.args.trackNo - 1) % numOfTracks;
                // reset tics to zero
                lcdBase->encoder.reset();
                // print title, and music tracks
                lcdBase->lcd.printCentral(MenuTitle);
                lcdBase->lcd.locate(0,1);
                lcdBase->lcd.printf("%s", flashPlayer.tracks[0].Title);
            }

            // check if button is pressed
            Button::state state = lcdBase->button.checkNewPresses();

            // select active track on short press and start playing song
            if (state == Button::state::Short_Press) 
            {
                playTrack();
            }
        
            // return to previous menu (unless no parent menu i.e. Main Menu) on long press
            else if((state == Button::state::Long_Press) && (parentMenu!= nullptr))  parentMenu->run();

            // sleep to ensure other threads have time to run
            ThisThread::sleep_for(10ms);
        }
    }

private:
    SDCardDriver SD;
    FlashPlayer flashPlayer;

    void playTrack()
    {
        // set custom characters
        lcdBase->lcd.setUDC(symbol::leftTrack, (char*)udc_leftTrack); 
        lcdBase->lcd.setUDC(symbol::leftTrackInv, (char*)udc_leftTrackInv); 
        lcdBase->lcd.setUDC(symbol::play, (char*)udc_play); 
        lcdBase->lcd.setUDC(symbol::playInv, (char*)udc_playInv); 
        // lcdBase->lcd.setUDC(symbol::pause, (char*)udc_pause); 
        // lcdBase->lcd.setUDC(symbol::pauseInv, (char*)udc_pauseInv); 
        lcdBase->lcd.setUDC(symbol::rightTrack, (char*)udc_rightTrack); 
        lcdBase->lcd.setUDC(symbol::rightTrack, (char*)udc_rightTrackInv); 


        // print music title
        lcdBase->lcd.printCentral(flashPlayer.tracks[flashPlayer.args.trackNo].Title);
        int activeSlection = symbol::play;
        charSelector(static_cast<symbol>(activeSlection));
    

        // while user is playing music
        while(true)
        {
            // if user rotates encoder, scroll through tracks
            if((lcdBase->encoder.getMechanicalTics()) != 0)
            {
                // increment or decrement through selections
                activeSlection += lcdBase->encoder.getMechanicalTics();
                // reset encoder tics to zero
                lcdBase->encoder.reset();
                // set limits for scrolling
                if(activeSlection < symbol::leftTrackInv) activeSlection = symbol::leftTrack;
                else if (activeSlection > symbol::rightTrack) activeSlection = rightTrack;

                // select option
                charSelector(static_cast<symbol>(activeSlection));
            }

            // check if button is pressed
            Button::state state = lcdBase->button.checkNewPresses();

            // select current selector
            if (state == Button::state::Short_Press) 
            {
                switch (activeSlection) 
                {
                    // go to previous song
                    case symbol::leftTrack:
                        flashPlayer.args.trackNo = (flashPlayer.args.trackNo - 1) % numOfTracks;
                        break;
                    // go to next song
                    case symbol::rightTrack:
                        flashPlayer.args.trackNo = (flashPlayer.args.trackNo + 1) % numOfTracks;
                        break;
                    // pause music
                    case symbol::play:
                        flashPlayer.args.pauseFlag = !flashPlayer.args.pauseFlag;
                        break;                
                }
            }
        
            // return to previous menu (unless no parent menu i.e. Main Menu) on long press
            else if((state == Button::state::Long_Press) && (parentMenu!= nullptr)) 
            {
                flashPlayer.deinit();
                break;
            }
        }
    }

    #define leftTrackCursor 1
    #define playCursor 7
    #define rightTrackCursor 15

    void charSelector(symbol selection)
    {
        switch (selection) 
        {
            case symbol::play:
                lcdBase->lcd.locate(leftTrackCursor,1);
                lcdBase->lcd.putc(symbol::leftTrack);
                lcdBase->lcd.locate(playCursor, 1);
                lcdBase->lcd.putc(symbol::playInv);
                lcdBase->lcd.locate(rightTrackCursor,1);
                lcdBase->lcd.putc(symbol::rightTrack);
                break;
            case symbol::leftTrackInv:
                lcdBase->lcd.locate(leftTrackCursor,1);
                lcdBase->lcd.putc(symbol::leftTrackInv);
                lcdBase->lcd.locate(playCursor, 1);
                lcdBase->lcd.putc(symbol::play);
                lcdBase->lcd.locate(rightTrackCursor,1);
                lcdBase->lcd.putc(symbol::rightTrack);
                break;
            case symbol::rightTrack:
                lcdBase->lcd.locate(leftTrackCursor,1);
                lcdBase->lcd.putc(symbol::leftTrack);
                lcdBase->lcd.locate(playCursor, 1);
                lcdBase->lcd.putc(symbol::play);
                lcdBase->lcd.locate(rightTrackCursor,1);
                lcdBase->lcd.putc(symbol::rightTrackInv);
                break;
            default:
                break;
        }
    }

};