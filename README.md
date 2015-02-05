#<center>WSN@amrita {Logging}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)
*Read the wiki before cloning*  
**Don't forget to update this README in every major commit!**

---
Implement ADC polling and EEPROM logging. Develop in stages, ADCtest followed by EEPROMtest and then merge into logging.

Description
--------------
ADCtest.c and SerControlADC.py can be used to test out the ADC. ADC is polled whenever `sFlag` (senseFlag) is set. Read value is sent over serial port after ~2ms (next processor wake, *this can be easily changed*) as it is (int between 0, 1023) where 1023 denotes Vcc of the device. This value is converted to volts in SerControlADC.py and displayed.
For now, only one value is read, expansion to (max) 6 channels is trivial.

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
