# imports
import GUI_frame, serial, myThread
# defines
gui = GUI_frame.Application()

# PUT THE PORT_ID HERE
PORT = '/dev/ttyUSB0'
gui.serUP = False
ser = serial.Serial()

io = myThread.ioThread(gui.q, ser, gui)

#logfile = open('logfile', 'w')

def closeSer():
  ser.flushInput()
  ser.flushOutput()
  ser.close()
  gui.serUP = False

def connect():
  try:
    ser.port = PORT
    ser.baudrate = 9600
    ser.timeout = None
    ser.open()
    gui.serUP = True
    try:
      #start thread to listen on port
      io.start()
      #start polling function
      gui.processQ()
      gui.right_put('^_^ Opened %s, now listening for messages!' % PORT)
    except:
      gui.right_put('*_* Some thread error, restart propram!')
  except:
    gui.right_put('*_* Error in setting up port @ "%s".\n*_* Try another port (launch Arduino IDE)' % PORT)

def disconnect():
  if gui.serUP:
    closeSer()
    gui.right_put('^_^ Closed port: %s' % PORT)
    gui.q.put('^_^ Session closed normally --------------\n')
    #logfile.close()
    io.end()

def parse(event):
  cmd = gui.cmd.get()
  if not cmd=='$ ':
    # log the command onto the pink pane
    gui.right_put(cmd)
    gui.cmd.delete('2', 'end')
    if gui.serUP:
      ser.write(cmd[2:])
    else:
      gui.right_put('*_* First connect to the Serial Port!')

###################################

gui.setup(connect, disconnect, parse)
gui.master.title('Test Window')
gui.mainloop()
