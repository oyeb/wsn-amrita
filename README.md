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
The program reads the port when `rFlag` is set.

About the GUI
--------------
The file `SerControl.py` is a framework for making a GUI to simultaneously

1. view Port incoming data *and*
2. send commands

On the terminal, we can't do this simultaneously since asking for user input blocks polling of the port data. The program needs to spawn another thread to ask for user input and let the OS handle the rest.
It's better to make a GUI with 2 panes and a command entry box.

The GUI is now stable and functional! But, from a holistic point of view, it **does not use Threads in the correct manner**. Care must be taken when using it, the wrong implementation is, I repeat, functional though.

####BUGS
Terminal that is used to open this Program gets blocked even though the GUI window has been closed. That's because:

- Thread for polling port does not properly terminate, even if [Disconnect] is pressed.
```
Every [Connect]-[Disconnect] (without killing the GUI, or in other words, in a single session) cycle spawns a new thread!!
It is recomended that you kill the terminal (killing the GUI is not enough) after a few cycles, say 3.
```


Build Instructions
-------------------
1. Upload serial.c using Arduino IDE
2. Note the portID (in the bottom-right corner of ARDUINO IDE) used by the freeduino, for ex, it could be `/dev/tty/USB0`.
   Copy that into `serport.py` as indicated in the comments there.
3. [*Not Recommended*]
    Run serport.py using
    `<your working dir>$ python serport.py 1`
4. If instead you wish to use the GUI to send commands to the device as well, here's what you do:
    `<your working dir>$ python SerControl.py`
    
