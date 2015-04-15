#<center>WSN@amrita {Logging}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)
*Read the [wiki](https://github.com/arrow-/wsn-amrita/wiki) before cloning*
**Don't forget to update this README in every major commit!**

---
Implement ADC polling and logging structures. EEPROM should be used only to take a backup on low-power. Reset on USB can be disabled by placing a capacitor (250uF) across Reset and GND.

1. ~~Build~~ Built a Queue of logs with MAXSIZE and each entry can be a struct (which is guaranteed to be tightly packed) with custom data members w/ or w/o compression. Data allocation, etc will be handled by `malloc()` and `free()`.
2. ~~Provide~~ Provides an upload feature (when char 'd' is recieved on the port).
3. Backup on low power and shutdown?

Should we write dhms (time) to EEPROM?

Description
--------------
`logger_0.c` regularly polls the ADC [1 channel for now, upgrade is trivial] and stores the value in a circular Queue. When 'd' is recieved on the serial port, the device dumps the Queue and the time variables.

Use `GUI_main.py` as the serial monitor. This is the modularised GUI [refer GUI wiki]. `myThread.py` dumps the readings into a list and labels the data with the correct timestamp. The whole dataset is then shown chronologically on the left-pane.

`collections` contains various `myThread.listen()` methods which can be used as example and tutorial programs.

TODO
---------
* Modularise `logger_0.c` into `logger.c`.
* Document GUI
* Make a GUI tutorial
* Setup makefile for multiple header use

Build Instructions
-------------------
1. Upload logger.c using Arduino IDE
2. Note the portID (in the bottom-right corner of ARDUINO IDE) used by the freeduino, for ex, it could be `/dev/tty/USB0`.
   Copy that into `GUI_main.py` as indicated in the comments there.
4. Then, `<your working dir>$ python GUI_main.py`
5. On-board LED blinks every time a value is sensed. Send 'd' to download data. Downloaded data should be visible on the left pane.
6. You can change the `Q_SIZE` and frequency of logging in `logger.c`
