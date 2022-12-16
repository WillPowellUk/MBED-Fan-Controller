/*  Author: CMULAY
    https://github.com/cmulay/Arduino-UNO-Dinosaur-Game
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#include "DinoGame.hpp"
#include "Settings.h"
#include "ButtonState.h"
#include "Utilities.hpp"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>

DinoGame::DinoGame(ILCDDriver &lcd, ButtonHandler &button, uint16_t* updateTime_ms)
    : lcd(lcd)
    , button(button)
    , updateTime_ms(updateTime_ms)
{
}

/*   An array of byte type is used to display
      bitmaps in an LCD Display.
      Here, the display is 5x8, the size of one
      'unit' in our lcd
      The places where it is a 1 will light up and
      the places with a 0 will be off
       You can create this array easily with a site
       that I'll explain below
*/

// bitmap array for the dino character
const char dino[]
{
    0b00000,
    0b00111,
    0b00101,
    0b10111,
    0b11100,
    0b11111,
    0b01101,
    0b01100,
};

// character for the tree
const char tree[]
{
    0b00011,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b11111,
    0b01110,
    0b01110
};


void DinoGame::run()
{
    exitGame = false;
    playState = false;

    while (true)
    {
        // when the button is pushed, print the dino on the upper
        // otherwise print the starting instruction
        Button::state state = button.checkNewPresses();

        // exit game on long press
        if (state == Button::state::Long_Press)
                exitGame = true;

        if(exitGame)
            return;

        if(!playState)
        {
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("Press the button");
            lcd.locate(0, 1);
            lcd.printf("to start: ");


            if (state == Button::state::Short_Press)
                playState = true;
        }

        if (playState)
            startGame();

        ThisThread::sleep_for(LCDUIYieldTime);
    }
}

// when playState is true, startGame function is called

void DinoGame::startGame()
{
    static int highScore = 0; 

    //declare a bool to check if the dino is in the ground
    bool dinoOnGround = true;  

    // variables to set the distance between the trees
    int dist = 0;   
    int distTwo = 0;

    int score = 0;

    // Set the UDCs
    lcd.setUDC(7, (char*)dino); 
    lcd.setUDC(6, (char*)tree); 

    lcd.cls();

    dist = Utilities::random(4, 9);
    distTwo = Utilities::random(4, 9);
    // Generate two random distances for the gap between the trees

    // this for loop is to make the trees move
    for (int i = 16; i >= -(dist + distTwo); i--)
    {

        lcd.locate(13, 0);
        lcd.printf("%i", score); // print the score

        // when the button is pushed, print the dino on the upper
        Button::state state = button.checkNewPresses();

        if(state == Button::state::Long_Press)
        {
            exitGame = true;
            return;
        }

        if (state == Button::state::Short_Press)
        {
            lcd.locate(1, 0); // row and set dinoOnGround to false
            lcd.putc(7);
            lcd.locate(1, 1);
            lcd.printf(" ");
            dinoOnGround = false;
        }
        else
        {                        // When the button is not pushed
            lcd.locate(1, 1); // print the dino on the lower row
            lcd.putc(7);        // and set dinoOnGround to true
            lcd.locate(1, 0);
            lcd.printf(" ");
            dinoOnGround = true;
        }

        // This prints the trees so that it moves with the for loop
        lcd.locate(i, 1);
        lcd.putc(6);
        lcd.locate(i + 1, 1);
        lcd.printf(" ");

        lcd.locate(i + dist, 1);
        lcd.putc(6);
        lcd.locate(i + dist + 1, 1);
        lcd.printf(" ");

        lcd.locate(i + dist + distTwo, 1);
        lcd.putc(6);
        lcd.locate(i + dist + distTwo + 1, 1);
        lcd.printf(" ");

        // When the trees reaches the starting point
        if ((i + dist + distTwo) == -1)
        {
            i = 12;
        } // they are taken to the end and set up randomly once more

        /*  Now check if the position of the trees
              and that of the dino coincides, but we
              only need to do this if the dino is in the ground.

             Use the dinoOnGround variable and check if the
             condition is satisfied. If so the game is over
        */

        if (i == 1 && (dinoOnGround == true))
        {
            lcd.cls();
            lcd.printf("Game Over!!!");
            if (score > highScore)
            {
                highScore = score;
            }

            lcd.locate(0, 1);
            lcd.printf("HS: ");
            lcd.printf("%i", highScore);
            playState = false; // Set the playState as false so it goes to the
            ThisThread::sleep_for(5000ms);       // start screen after the game is over
            score = 0;
            break;
        }
        else if (i + dist == 1 && (dinoOnGround == true))
        {
            lcd.cls();
            lcd.printf("Game Over!!!");
            if (score > highScore)
            {
                highScore = score;
            }
            lcd.locate(0, 1);
            lcd.printf("HS: ");
            lcd.printf("%i", highScore);
            playState = false;
            ThisThread::sleep_for(5000ms);
            score = 0;
            break;
        }
        else if (i + dist + distTwo == 1 && (dinoOnGround == true))
        {
            lcd.cls();
            lcd.printf("Game Over!!!");
            if (score > highScore)
            {
                highScore = score;
            }
            lcd.locate(0, 1);
            lcd.printf("HS: ");
            lcd.printf("%i", highScore);
            playState = false;
            ThisThread::sleep_for(5000ms);
            score = 0;
            break;
        }

        // increase the score variable for every run of the loop
        score++;

        ThisThread::sleep_for(std::chrono::milliseconds(*updateTime_ms));
    }
}