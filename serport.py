# This program will represent all data packets as ints, even if your c-code sends chars
# * make this general, ie accept chars etc?

import serial, argparse, time
#start a argument parser, we don't need this now but may need it later
parser = argparse.ArgumentParser(description="Plot data from arduino, specify frequency of data.")
parser.add_argument("rate", help="Packet interval in milliseconds", type=int)
args = parser.parse_args()
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=None)

#main reading stuff
while True:
  data = ser.read()
  if not data == '':	#enter if-block only if some data has been sent to PC
    sz = ord(data)	#ord converts char to int
    info = 0		#actual data sent in packet (currently only working with ints)
    for i in range(0,sz):
      data = ser.read()
      info += ord(data)*(256**i)	#some trivial math
    if info>60000:
      print '*', info
    else:
      print info
          
     
