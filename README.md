# MBED-Fan-Controller
Built for the STM32F070xx microcontroller using Mbed OS

Since most of my projects are kept under the hood in private repositories, I thought I'd share this Uni project which doesn't have any NDAs.

**Check out the design document [here](https://github.com/WillPowellUk/MBED-Fan-Controller/blob/master/Fan%20Controller%20Design%20Documet%20-%20William%20Powell.pdf)**


## The main features
* Fan controller using closed loop control methods (PID and LQR)
* User Interface on a 16x2 LCD
* Data logger using an SD card
* Music Player using coil whine and the SD card

## The nerdy stuff
* Using Mbed's RTOS for multithreading (tasks), queues etc.
* Interface classes used to detach dependencies on drivers

