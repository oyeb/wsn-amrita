#<center>WSN@amrita</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)

How to contribute
----------------------
- Install Arduino IDE ***(preffered)*** for your environment [here][1], or use [Atmel Studio][2] on Windows.
- All features should be developed in branches. Commits should be made regularly so that each feature has a rich *file history*.
- Once a feature is completed, merge it with other branch(es) making a file that implements more than one feature. Test this thoroughly and then commit this *Merged File*.
- Include Build and Run instructions in the file. Prefferd setup:
  * Open C file with Arduino IDE.
  * Compile and Upload
  * Test code!

Current Branches
----------------------
###Timer
Implements a Software Timer of 2 msec resolution using *TIMER2*. MCU sleeps in idle mode for *>90%* time.
**Future Work**
Use RTC (32.768kHz) of lowest possible resolution to maximise power-saving. Using RTC a deeper power-save mode can be invoked.
*Low interrupt freq. + deep sleep = ⇑Max power saving, though this requires hardware changes too*.

###Serial
Implement a serial communication interface between PC and node. 2-way message support. Respond to PC's requests *aka.* commands. Periodically send data packets to PC

###Sensor Log + EEPROM
Log sensor data into EEPROM. Determine how long a device can hold log-data before an upload to PC.
***Persistent memory == EEPROM == very difficult***  

[1]: http://arduino.cc/en/Main/Software 
[2]: http://www.atmel.com/tools/atmelstudio.aspx
