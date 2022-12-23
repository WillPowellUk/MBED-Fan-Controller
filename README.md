# MBED-Fan-Controller
Built for the STM32F070xx microcontroller using Mbed OS (not baremetal this time)!

Since most of my projects are kept under the hood in private repositories, I thought I'd share this Uni project which doesn't have any NDAs.

**Check out the design document [here](https://github.com/WillPowellUk/MBED-Fan-Controller/blob/master/Fan%20Controller%20Design%20Documet%20-%20William%20Powell.pdf)**


## The main features
* Fan controller using closed loop control methods (PID, PD, PI, P control)
* Music Player using coil whine and the SD card
* User Interface on a 16x2 LCD
* Dinosaur Game for entertainment purposes

## User Instructions
* To navigate through the LCD menu, scroll encoder and short press to select current option
* Long press to navigate to previous menu
* If the device freezes, a thread error may have occured. Please reset board.

## The nerdy stuff
* Using Mbed's RTOS for multithreading (tasks), queues, events, mutexes...
* Interface classes used to detach dependencies on drivers

## Legal
* Author's name is specified on each header and source file
* All files are assumed to be open source if unspecified
