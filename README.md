#<center>WSN@amrita {Serial}</center>
[//]: # (This is a comment)
[//]: # (This document is written in markdown. I recommend editing this on Github or markdown- here.com/livedemo.html)
*Read the wiki before cloning*  
**Don't forget to update this README in every major commit!**

---
Implement a serial communication interface between PC and node. 2-way message support. Respond to PC's requests *aka.* commands. Periodically send data packets to PC.
Hence, development of PC-side interfacing application is also included under this branch.

Description
--------------
serial.c only transits Data Packets and serport.py, a Python script runs on the PC to parse the packets and display on the console.
System Clock 16MHz sourced from external Crystal Oscillator  which is on-board.
Implements USART (transit only) in the Data Packet Format explained in the wiki and serial.c
Thus totally **sz+1** bytes are required.
This format could be easily changed to *fixed size* format, if needed.
####USES:
  PORTD 0,1 (PORTD0-1){RX and TX}
  as USART pins

The program writes 'count' every 2msec (when `wFlag` is set) and decrements it.

Build Instructions
----------------------
1. Upload serial.c using Arduino IDE
2. Note the portID (in the bottom-right corner of ARDUINO IDE) used by the freeduino, for ex, it could be `/dev/tty/USB0`.
   Copy that into `serport.py` as indicated in the comments there.
3. Run serport.py using
    `<your working dir>$ python serport.py 1`
    
