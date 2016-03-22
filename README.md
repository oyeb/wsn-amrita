#<center>WSN@amrita {TIMER}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)

AIM
===
This project aims to develop a lightweight, non-obtrusive interface to an XBEE + Arduino + Sensor couple. Both for the PC and functional interfaces for the Arduino. The `timer`, `serial` interface (PC also) are ready.

The work of this project has enabled implentation of a cluster of 9 *(that's small!)** WSN nodes that have sensors to measure plant health, soil condition. The team wants to explore different network configurations and test (possibly novel) communication protocols on the framework.

The `master` does not have the logging GUI, see the `logging` branch, the GUI is stable.

Build Process
---------
Refer the wiki for more details on the [build process](wiki/Building).

Usage Guide
-------------
The time variables can be accessed by inserting this piece of code somewhere in the beggining of your main-file:
```
extern volatile uint8_t m,h,d,s;
```
Since the Scheduler is not ready yet, You have to edit the ISR in `utimer.c` to perform the scheduled tasks. The sample `timer-main.c`'s scheduled task is to blink an LED.

**Future Work**

* Scheduler needs to be modularised.
* Use RTC (32.768kHz) of lowest possible resolution to maximise power-saving. Using RTC a deeper power-save mode can be invoked. *Low interrupt freq. + deep sleep = ⇑Max power saving, though this requires hardware changes too*.

Description
-----------
System Clock is 16MHz sourced from external Crystal Oscillator which is on-board.
Counter is updated with SYS_clock(16MHz) prescaled by 128, thus `Freq_TIMER2 = 16MHz/128 = 125kHz`<br> TIMER2 interrupts after every 250 (OCR2 == 250) clock pulses, so `1/125kHz x 250 = 2msec`

* `ms, s, m, h, d` are not global TIME variables in `utimer.c`
* `utimer.c` holds the ISR and initialization routine for the timer. `timer-main.c` just includes the ISR [in future this will become a scheduler].

####USES:
  PIN 2-7(PORTD2-7), 13(PORTB5)<br>as O/P

For visual response, 13th Arduino Digital I/O pin is set as output to drive on-board LED. LED flashes briefly every second and for 1 second every 60 secs.
If LEDs are connected to PIN2 - PIN7, one can see real time value of variable 's' (PIN2 is LSB). Of course that's only for demonstration.

TODO
---------
* Build Scheduler
* Develop makefile

Build Instructions [NEW]
------------------
1. Create you 'timer-main' file, include `utimer.h` in it. Make the necessary changes in `utimer.c`.
2. Fire up the terminal to <your-dir> and the Arduino IDE to find which port is being used, eg: `/dev/ttyUSB0`.
3. Run `<your-dir>$ bash commands <portID> to compile and flash your code to the device.

Build Instructions [OLD]
------------------
Just open timerOLD.c (that is a test file) in Arduino IDE and upload to device. On-board LED should start blinking as decribed above!

####Notes
`bash commands [clean|<portID>]`
`clean` - remove all files dumped by the avr toolchain
`<portID>` - compile, link, assemble flash to `<portID>`
