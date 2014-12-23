import serial, argparse, time
parser = argparse.ArgumentParser(description="Plot data from arduino, specify frequency of data.")
parser.add_argument("rate", help="Packet interval in milliseconds", type=int)
args = parser.parse_args()
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=None)
time.sleep(10)
while True:
  data = ser.read()
  if not data == '':
    sz = ord(data)
    info = 0
    for i in range(0,sz):
      data = ser.read()
      info += ord(data)*(256**i)
    if info>60000:
      print '*', info
    else:
      print info
          
     
