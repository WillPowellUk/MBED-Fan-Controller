/*  Author: William Powell
    University of Bath
    December 2022
    
    Built for: STM32F070xx
    MBED-OS Version 6.16.0
*/


#include "FlashPlayer.hpp"
#include "mbed.h"
#include "Settings.h"
#include <chrono>
#include <cstdint>

FlashMusic Track1 = 
{
    .Title = "Track 1",
    .samplingFq = 1,
    .spectrogram = {600, 500, 1700, 400, 450, 0, 500, 600, 700, 600, 500, 1700, 400, 450, 0, 500, 600, 700}
};

FlashMusic Track2 = 
{
    .Title = "Track 2",
    .samplingFq = 1,
    .spectrogram = {600, 500, 1700, 400, 1700, 0, 500, 1000, 2000, 200, 500, 1700, 400, 450, 0, 500, 600, 700}
};

FlashMusic Track3 = 
{
    .Title = "Track 3",
    .samplingFq = 1,
    .spectrogram = {2000, 800, 1700, 400, 800, 1700, 400, 800, 1700, 400, 800, 1700, 1700, 400, 450, 0, 400, 600}
};


// define static member variables
FlashMusic FlashPlayer::tracks[numOfTracks]
{
    Track1, Track2, Track3
}; 

FlashPlayer::FlashPlayer(FanController& fan)
    : fan(fan)
{
    samplingRateTimer.start();
}


void FlashPlayer::play_NonBlocking(uint16_t* trackNo, bool* pauseMusicFlag)
{
    // exit immediately if music is paused
    if (*pauseMusicFlag) return;

    uint64_t timeElapsed_us = samplingRateTimer.elapsed_time().count();

    // exit and come back if not enough time has passed
    if(timeElapsed_us < (1.0e6/tracks[*trackNo].samplingFq)) return;
    samplingRateTimer.reset();
    samplingRateTimer.start();

    // set 80% duty cycle for faster acceleration
    fan.setDesiredSpeed_Percentage(0.8);

    // play frequencies one by one to make a song
    static int i=0;

    // if the track has finished, load the next one (infinite scrolling)
    if (i >= ((sizeof(tracks[*trackNo].spectrogram)) / sizeof(uint16_t)))
    {
        *(trackNo) = (*(trackNo) + 1) % numOfTracks;
        i = 0;
    }
    
    // set pwm out based on current index in spectrogram
    fan.setPWMOutFrequency_Hz(tracks[*trackNo].spectrogram[i]);

    // increment spectrogram index
    i++;

}