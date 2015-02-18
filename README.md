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

ADCTests successful, verified effectiveness of capacitor. Now work on Queue.

About the GUI
--------------
Refer README in branch **serial**.

Build Instructions
-------------------
1. Upload ADCtest.c using Arduino IDE
2. Note the portID (in the bottom-right corner of ARDUINO IDE) used by the freeduino, for ex, it could be `/dev/tty/USB0`.
   Copy that into `SerControlADC.py` as indicated in the comments there.
4. Then, `<your working dir>$ python SerControlADC.py`
5. You should see some values on the left pane once the device boots up.
