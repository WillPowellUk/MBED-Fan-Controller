#include "FlashPlayer.hpp"
#include "mbed.h"
#include "Settings.h"
#include <cstdint>


FlashMusic Track1 = 
{
    .Title = "Track 1",
    .samplingFq = 2,
    .frequencies = {600, 500, 1700, 400, 450, 0, 500, 600, 700}
};

FlashMusic Track2 = 
{
    .Title = "Track 2",
    .samplingFq = 2,
    .frequencies = {600, 500, 1700, 400, 1700, 0, 500, 1000, 2000}
};

// define static member variables
FlashMusic FlashPlayer::tracks[numOfTracks]
{
    Track1, Track2
}; 
FlashPlayer::ThreadInputArgs FlashPlayer::args
{
    .trackNo = 0,
    .pauseFlag = false
};

FlashPlayer::FlashPlayer(FanController& fan)
    : fan(fan)
    , thread(osPriorityAboveNormal, 4096, nullptr, "Music Player")
{
}

void FlashPlayer::init()
{
    thread.start(callback(this, &FlashPlayer::play));
}

void FlashPlayer::deinit()
{
    thread.terminate();
}


void FlashPlayer::play()
{
    // calculate time period between frequencies
    float dt_ms = 1000.0/(tracks[args.trackNo].samplingFq);

    // set 50% duty cycle
    fan.setDesiredSpeed_Percentage(0.5);

    while (true)
    {
        // play frequencies one by one to make a song
        for(int i=0; i<sizeof(tracks[args.trackNo].frequencies); i++)
        {
            static uint16_t prevTrackNo = args.trackNo;
            // check flag to pause or skip track
            while(!args.pauseFlag) ThisThread::sleep_for(10ms);
            if (args.trackNo != prevTrackNo)
            {
                args.trackNo = (args.trackNo - 1) % numOfTracks;
                prevTrackNo = args.trackNo;
                break;
            }
            // set pwm out based on frequency
            fan.setPWMOutFq(tracks[args.trackNo].frequencies[i]);
            ThisThread::sleep_for(dt_ms);
        }
        // increment through to next song
        args.trackNo = (args.trackNo + 1) % numOfTracks;
        ThisThread::sleep_for(10ms);
    }

}