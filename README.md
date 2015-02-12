#<center>WSN@amrita</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)

How to contribute
----------------------
- Install Arduino IDE ***(preffered)*** for your environment [here][1], or use [Atmel Studio][2] on Windows.
- All features should be developed in branches. Commits should be made regularly so that each feature has a rich *file history*.
- Once a feature is completed, merge it with other branch(es) making a file that implements more than one feature. Test this thoroughly and then commit this *Merged File*.
- 0.0
  * Files are standalone .c file that need to built on the Arduino IDE.
  * Open file using IDE, build and upload
- 0.1
  * All basic utilities like Timer, Serial Interface, ADC etc. have been modularised. Each branch will have a Makefile, use that. Contents of the Makefile are discussed below.
  * Download all .c and .h files. Then invoke `make <Branch-Name>`

Current Branches
----------------------
###Timer
**0.0** Implements a Software Timer of 2 msec resolution using *TIMER2*. MCU sleeps in idle mode for *>90%* time.
**Future Work**
Use RTC (32.768kHz) of lowest possible resolution to maximise power-saving. Using RTC a deeper power-save mode can be invoked.
**0.1** timer-main.c is to be used as the scheduler.
*Low interrupt freq. + deep sleep = ⇑Max power saving, though this requires hardware changes too*.

###Serial
**0.0** Implement a serial communication interface between PC and node. 2-way message support. Respond to PC's requests *aka.* commands. Periodically send data packets to PC.

**0.1** Just include serial.h

###Logging
Log sensor data into EEPROM. Determine how long a device can hold log-data before an upload to PC.
ADC has been tested, need to implement EEPROM. Also need to decide the compression and info format.

Makefile
---------
First compile `MAIN.c`. Linking happens later.
```
avr-gcc -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega8 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ MAIN.c -o MAIN.o
```
Now compile all modules: utimer, userial, uadc, etc.
```
avr-gcc -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega8 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ utimer.c -o utimer.o
```
Add them all to our custom.
```
avr-ar rcs libavrutils.a utimer.o
```

Generate an elf and link against libavrutils.
```
avr-gcc -Os -Wl,--gc-sections -mmcu=atmega8 -o MAIN.elf MAIN.o libavrutils.a -L.
avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 MAIN.elf MAINeep
avr-objcopy -O ihex -R .eeprom MAIN.elf MAIN.hex
```

Flash the hex. *For this command you must install Arduino IDE.*
```
avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -v -patmega8 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:MAIN.hex:i
```


[1]: http://arduino.cc/en/Main/Software 
[2]: http://www.atmel.com/tools/atmelstudio.aspx
