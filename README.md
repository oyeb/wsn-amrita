#<center>WSN@amrita {Serial}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)
*Read the [wiki](https://github.com/arrow-/wsn-amrita/wiki) before cloning*
**Don't forget to update this README in every major commit!**

---
Implement a serial communication interface between PC and node.<br>2-way message support. Respond to PC's requests *aka.* commands. Periodically send data packets to PC.
*Hence, development of PC-side interfacing application is also included under this branch.*

Description
--------------
Implements USART (transmit only) in the Data Packet Format explained in the wiki and serial.c
Thus totally **sz+1** bytes are required. This can be used to test the USART abstraction.
This format could be easily changed to *fixed size* format, if needed.
####USES:
  PORTD 0,1 (PORTD0-1){RX and TX}
  as USART pins

About the GUI
--------------
The file `SerControl.py` a GUI to simultaneously,

1. view Port incoming data *and*
2. send commands

More info about the GUI can be found in the [wiki].<br>
**[TODO]** This python script is old and needs to be replaced with the modularised GUI. It would serve as a good example code on how to use the listen() method in the GUI.


Build Instructions
----------------------
*Your main file should be called `sermain.c`.* We'll remove that need once the makefiles are setup.

1. Fire up the terminal to <your-dir> and the Arduino IDE
2. Note the portID (in the bottom-right corner of ARDUINO IDE) used by the Arduino, for ex, it could be `/dev/tty/USB0`.
   - Copy that into `SerControl.py` as indicated in the comments there.
   - Run the shell script `commands` by `$<your-dir> bash commands <portID>`. This will compile your code and upload it to the Arduino.
3. To start the Serial Monitor, `<your working dir>$ python SerControl.py`
=======

***

####Notes
`bash commands [clean|<portID>]`
`clean` - remove all files dumped by the avr toolchain
`<portID>` - compile, link, assemble flash to `<portID>`
