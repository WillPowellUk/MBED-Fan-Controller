/* mbed TextLCD Library, for LCDs based on HD44780 controllers
 * Copyright (c) 2014, WH
 *               2014, v01: WH, Extracted from TextLCD.h as of v14
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef MBED_TEXTLCDUDC_H
#define MBED_TEXTLCDUDC_H

#include "TextLCD_Config.h"

/** Some sample User Defined Chars 5x7 dots */
#if(LCD_UDC == 1)

const char udc_fill[] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
const char udc_top_and_bottom_perimeter[] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
const char udc_left_perimeter[] = 
{
  0b11111,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b11111
};
const char udc_right_perimeter[] = 
{
  0b11111,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b11111
};

const char udc_fifth_full[] = 
{
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000
};

const char udc_two_fifths_full[] = 
{
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000
};

const char udc_three_fifths_full[] = 
{
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100
};

const char udc_four_fifths_full[] = 
{
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110
};


const char udc_play[] = 
{
  0b00000,
  0b01000,
  0b01100,
  0b01110,
  0b01110,
  0b01100,
  0b01000,
  0b00000
};


const char udc_playInv[] = 
{
  0b11111,
  0b10111,
  0b10011,
  0b10001,
  0b10001,
  0b10011,
  0b10111,
  0b11111
};


const char udc_leftTrack[] = 
{
  0b10001,
  0b10011,
  0b10111,
  0b11111,
  0b11111,
  0b10111,
  0b10011,
  0b10001 
};

const char udc_leftTrackInv[] = 
{
  0b01110,
  0b01100,
  0b01000,
  0b00000,
  0b00000,
  0b01000,
  0b01100,
  0b01110
};

const char udc_rightTrack[] = 
{
  0b10001,
  0b11001,
  0b11101,
  0b11111,
  0b11111,
  0b11101,
  0b11001,
  0b10001
};


const char udc_rightTrackInv[] = 
{
  0b01110,
  0b00110,
  0b00010,
  0b00000,
  0b00000,
  0b00010,
  0b00110,
  0b01110
};

const char udc_pause[] = 
{
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001
};

const char udc_pauseInv[] = 
{
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};


//extern const char udc_ae[];      //æ
//extern const char udc_0e[];      //ø
//extern const char udc_ao[];      //å
//extern const char udc_AE[];      //Æ
//extern const char udc_0E[];      //Ø
//extern const char udc_Ao[];      //Å
//extern const char udc_PO[];      //Padlock Open
//extern const char udc_PC[];      //Padlock Closed

//extern const char udc_alpha[];  //alpha
//extern const char udc_ohm[];    //ohm
//extern const char udc_sigma[];  //sigma
//extern const char udc_pi[];     //pi
//extern const char udc_root[];   //root

extern const char udc_0[];       // |>
extern const char udc_1[];       // <|
extern const char udc_2[];       // |
extern const char udc_3[];       // ||
extern const char udc_4[];       // |||
extern const char udc_5[];       // =
extern const char udc_6[];       // checkerboard
extern const char udc_7[];       // \

extern const char udc_degr[];    // Degree symbol

extern const char udc_TM_T[];    // Trademark T
extern const char udc_TM_M[];    // Trademark M

//extern const char udc_Bat_Hi[];  // Battery Full
//extern const char udc_Bat_Ha[];  // Battery Half
//extern const char udc_Bat_Lo[];  // Battery Low
extern const char udc_Bat_Hi[];  // Battery Full
extern const char udc_Bat_Ha[];  // Battery Half
extern const char udc_Bat_Lo[];  // Battery Low
extern const char udc_AC[];      // AC Power

extern const char udc_smiley[];  // Smiley
//extern const char udc_droopy[];  // Droopey
//extern const char udc_note[];    // Note

//extern const char udc_bar_1[];   // Bar 1
//extern const char udc_bar_2[];   // Bar 11
//extern const char udc_bar_3[];   // Bar 111
//extern const char udc_bar_4[];   // Bar 1111
//extern const char udc_bar_5[];   // Bar 11111

//extern const char udc_ch_1[];    // Hor bars 4
//extern const char udc_ch_2[];    // Hor bars 4 (inverted)
//extern const char udc_ch_3[];    // Ver bars 3
//extern const char udc_ch_4[];    // Ver bars 3 (inverted)
//extern const char udc_ch_yr[];   // Year   (kana)
//extern const char udc_ch_mo[];   // Month  (kana)
//extern const char udc_ch_dy[];   // Day    (kana)
//extern const char udc_ch_mi[];   // minute (kana)
extern const char udc_None[]; 
extern const char udc_All[];
#endif /* LCD_UDC Examples */

#endif // MBED_TEXTLCDUDC_H