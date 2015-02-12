#<center>WSN@amrita {TIMER}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)

*Read the wiki before cloning!*  
**Don't forget to update this README in each commit!**

----
Implements a Software Timer of 2 msec resolution using *TIMER2*. MCU sleeps in idle mode for *>90%* time.

Scheduler has been modularised. Just make the schedule changes in `utimer.c` and include `utimer.h` in the "MAIN" code and setup the Makefile for your "MAIN".

**Future Work**
Use RTC (32.768kHz) of lowest possible resolution to maximise power-saving. Using RTC a deeper power-save mode can be invoked.
*Low interrupt freq. + deep sleep = ⇑Max power saving, though this requires hardware changes too*.

Description
-----------
* System Clock is 16MHz sourced from external Crystal Oscillator  which is on-board.
* Counter updated with SYS_clock(16MHz) prescaled by 128
   * Freq_TIMER2 = 16MHz/128 = 125kHz

TIMER2 interrupts after every 250 (OCR2 == 250) clock pulses,

  | 1 pulse = 1/125kHz | 8 usec |
  | --- | ---:|
  | 250 pulses | 2 msec |
* `ms, s, m, h, d` are not global TIME variables.
* `utimer.c` holds the ISR and initialization routine for the timer. `timer-main.c` just includes the "Scheduler".

####USES:
  PIN 2-7(PORTD2-7), 13(PORTB5)
  as O/P

For visual response, 13th Arduino Digital I/O pin is set as output to drive on-board LED. LED flashes briefly every second and for 1 second every 60 secs.
If LEDs are connected to PIN2 - PIN7, one can see real time value of variable 's' (PIN2 is LSB).

Build Instructions
------------------
Just open timerOLD.c (that is a test file) in Arduino IDE and upload to device. On-board LED should start blinking as decribed above!
Actual Instructions for building the project:

For visual response, 13th Arduino Digital I/O pin is set as output to drive on-board LED. LED flashes briefly every second and for 1 second every 60 secs.
If LEDs are connected to PIN2 - PIN7, one can see real time value of variable 's' (PIN2 is LSB).

Build Instructions
------------------
Just open timerOLD.c (that is a test file) in Arduino IDE and upload to device. On-board LED should start blinking as decribed above!

Actual Instructions for building the project:

1. Run `make timer-main`.
2. To flash the code run `make flash`.
