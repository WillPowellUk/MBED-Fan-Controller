#include "FlashPlayer.hpp"
#include "mbed.h"
#include "Settings.h"
#include <chrono>
#include <cstdint>


// FlashMusic Track1 = 
// {
//     .Title = "Track 1",
//     .samplingFq = 2,
//     .spectrogram = {600, 500, 1700, 400, 450, 0, 500, 600, 700}
// };

// FlashMusic Track2 = 
// {
//     .Title = "Track 2",
//     .samplingFq = 1,
//     .spectrogram = {600, 500, 1700, 400, 1700, 0, 500, 1000, 2000}
// };

// define static member variables
FlashMusic FlashPlayer::tracks[numOfTracks]
{
    // Track1, Track2
}; 

FlashPlayer::FlashPlayer(FanController& fan)
    : fan(fan)
{
}


void FlashPlayer::play_NonBlocking(uint16_t* trackNo, bool* pauseMusicFlag)
{
    // exit immediately if music is paused
    if (*pauseMusicFlag) return;

    // timer to prevent blocking wait for sampling frequency
    static Timer samplingRateTimer;
    float timeElapsed_s = (samplingRateTimer.elapsed_time().count()) / 1.0e6;

    // exit and come back if not enough time has passed
    if(timeElapsed_s < (1.0/tracks[*trackNo].samplingFq)) return;
    samplingRateTimer.reset();
    samplingRateTimer.start();

    // set 50% duty cycle
    fan.setDesiredSpeed_Percentage(0.5);

    // play frequencies one by one to make a song
    static int i=0;

    // if the track has finished, load the next one (infinite scrolling)
    if (i > (sizeof(tracks[*trackNo].spectrogram)))
    {
        *(trackNo) = *(trackNo + 1) % numOfTracks;
        i = 0;
    }
    
    // set pwm out based on current index in spectrogram
    fan.setPWMOutFrequency_Hz(tracks[*trackNo].spectrogram[i]);

    // increment spectrogram index
    i++;

}