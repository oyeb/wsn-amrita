#<center>WSN@amrita</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)
Please refer the [wiki](https://github.com/arrow-/wsn-amrita/wiki)
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

Makefile
---------
Refer the wiki for more details on the build process.

1. First compile `MAIN.c`. Linking happens later.
```
avr-gcc -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega8 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ MAIN.c -o MAIN.o
```
2. Now compile all modules: utimer, userial, uadc, etc.
```
avr-gcc -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega8 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ utimer.c -o utimer.o
```
3. Add them all to our custom library called `libavrutils.a`.
```
avr-ar rcs libavrutils.a utimer.o
```

4. Generate a `.elf` and link against `libavrutils.a`.
```
avr-gcc -Os -Wl,--gc-sections -mmcu=atmega8 -o MAIN.elf MAIN.o libavrutils.a -L.
avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 MAIN.elf MAINeep
avr-objcopy -O ihex -R .eeprom MAIN.elf MAIN.hex
```
5. Flash the hex *(for ATmega8 boards)*. *For this command you must install Arduino IDE.*
```
avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -v -patmega8 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:MAIN.hex:i
```


[1]: http://arduino.cc/en/Main/Software 
[2]: http://www.atmel.com/tools/atmelstudio.aspx
