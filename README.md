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

[1]: http://arduino.cc/en/Main/Software 
[2]: http://www.atmel.com/tools/atmelstudio.aspx
