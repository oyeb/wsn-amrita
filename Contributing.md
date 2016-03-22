###Please refer the [wiki](https://github.com/arrow-/wsn-amrita/wiki) to know about the project.
How to contribute
----------------------
- Install Arduino IDE ***(preffered)*** for your environment [here][1], or use [Atmel Studio][2] on Windows.
- All features should be developed in branches. Commits should be made regularly so that each feature has a rich *file history*.
- Once a feature is completed, merge it with other branch(es) making a file that implements more than one feature. Test this thoroughly on your board and then commit this *Merged File*.

Current Branches
----------------
**We still work in branches. `master` does not have the most functional module, the `GUI_main.py` (it's in `logging` branch).**

###Timer
Implements a Software Timer of 2 msec resolution using *TIMER2*. MCU sleeps in idle mode for *>90%* time. Sleep modes dont work on ATmega8 boards.
**Future Work**

* ~~Modularise, make header-source files~~ [completed]
* Use RTC (32.768kHz) of lowest possible resolution to maximise power-saving. Using RTC a deeper power-save mode can be invoked. *Low interrupt freq. + deep sleep = ⇑Max power saving, though this requires hardware changes too*.

###Serial
Implement a serial communication interface between PC and node. 2-way message support. Respond to PC's requests *aka.* commands. Periodically send data packets to PC.

**Future Work**
* ~~Modularise, make header-source files~~ [completed]
* Extend into `Robust-Serial`.

###Logging
Log sensor data into EEPROM when low on power.
* Determine how long a device can hold log-data before an upload to PC.
* ADC has been tested, implemented EEPROM.
* ~~Need to decide the compression and data format~~ [completed].
* Nodes can be tasked to their job standalone! Once in ever ~37 hours, the memory bank gets filled. Need to use the `GUI Module/GUI_main.py` to extract all the data.