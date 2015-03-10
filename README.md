#<center>WSN@amrita {Logging}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)
*Read the wiki before cloning*  
**Don't forget to update this README in every major commit!**

---
Implement ADC polling and logging structures. EEPROM should be used only to take a backup on low-power. Reset on USB can be disabled by placing a capacitor (250uF) across Reset and GND.

1. Build a Queue of logs with MAXSIZE and each entry can be a struct (which is guaranteed to be tightly packed) with custom data members w/ or w/o compression. Data allocation, etc will be handled by `malloc()` and `free()`.
2. Provide an upload feature.
3. Backup on low power and shutdown?

Should we write dhms (time) to EEPROM?

Description
--------------
ADCtest.c and SerControlADC.py can be used to test out the ADC. ADC is polled whenever `sFlag` (senseFlag) is set. Read value is sent over serial port after ~2ms (next processor wake, *this can be easily changed*) as it is (int between 0, 1023) where 1023 denotes Vcc of the device. This value is converted to volts in SerControlADC.py and displayed.
For now, only one value is read, expansion to (max) 6 channels is trivial.

ADCTests successful, verified effectiveness of capacitor. `logger.c` now boasts of a queue based system for logging sensor data. The 

About the GUI
--------------
Refer README in branch **serial**.
 `logger.c` maintains a Queue of most recent logged values. Value sensed every second. `Q_SIZE = 10`.
 `PassiveLogger.py` opens a logfile and downloads data from arduino when 'd' is sent to the board. Name of logfile is `logfile`.

Build Instructions
-------------------
1. Upload logger.c using Arduino IDE
2. Note the portID (in the bottom-right corner of ARDUINO IDE) used by the freeduino, for ex, it could be `/dev/tty/USB0`.
   Copy that into `PassiveLogger.py` as indicated in the comments there.
4. Then, `<your working dir>$ python PassiveLogger.py`
5. On-board LED blinks every time a value is sensed. Send 'd' to download data. Downloaded data should be visible on the left pane.
6. You can change the `Q_SIZE` and frequency of logging in `logger.c`
