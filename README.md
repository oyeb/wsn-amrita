#<center>WSN@amrita {TIMER}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)

###Please refer the [wiki](https://github.com/arrow-/wsn-amrita/wiki) to know about the project.
How to contribute
----------------------
- Install Arduino IDE ***(preffered)*** for your environment [here][1], or use [Atmel Studio][2] on Windows.
- All features should be developed in branches. Commits should be made regularly so that each feature has a rich *file history*.
- Once a feature is completed, merge it with other branch(es) making a file that implements more than one feature. Test this thoroughly on your board and then commit this *Merged File*.

Current Branches
----------------------
###Timer
Implements a Software Timer of 2 msec resolution using *TIMER2*. MCU sleeps in idle mode for *>90%* time. Sleep modes dont work on ATmega8 boards.
**Future Work**

* ~~Modularise, make header-source files~~ [completed]
* Use RTC (32.768kHz) of lowest possible resolution to maximise power-saving. Using RTC a deeper power-save mode can be invoked. *Low interrupt freq. + deep sleep = ⇑Max power saving, though this requires hardware changes too*.

###Serial
Implement a serial communication interface between PC and node. 2-way message support. Respond to PC's requests *aka.* commands. Periodically send data packets to PC.

**Future Work**
* ~~Modularise, make header-source files~~ [completed]
* Extend into `Robust-Serial`.

###Logging
Log sensor data into EEPROM when low on power.
* Determine how long a device can hold log-data before an upload to PC.
* ADC has been tested, need to implement EEPROM.
* ~~Need to decide the compression and data format~~ [completed].

Build Process
---------
Refer the wiki for more details on the [build process].

1. First compile `MAIN.c`. Linking happens later.<br>
`avr-gcc -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega8 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ MAIN.c -o MAIN.o`
2. Now compile all modules: utimer, userial, uadc, etc.<br>
`avr-gcc -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega8 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ utimer.c -o utimer.o`
3. Add them all to our custom library called `libavrutils.a`.<br>
`avr-ar rcs libavrutils.a utimer.o`
4. Generate a `.elf` and link against `libavrutils.a`.<br>
`avr-gcc -Os -Wl,--gc-sections -mmcu=atmega8 -o MAIN.elf MAIN.o libavrutils.a -L.`<br>
`avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 MAIN.elf MAINeep`<br>
`avr-objcopy -O ihex -R .eeprom MAIN.elf MAIN.hex`
5. Flash the hex *(for ATmega8 boards)*. *For this command you must install Arduino IDE.*<br>
`avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -v -patmega8 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:MAIN.hex:i`


*Read the [wiki](https://github.com/arrow-/wsn-amrita/wiki) before cloning!*
**Don't forget to update this README in each commit!**

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
