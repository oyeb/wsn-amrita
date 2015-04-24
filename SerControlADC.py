'''
 Opens a GUI window with 2 panes
 Left pane is to view the data incoming from the connected device.
  It is scroll-able
 Right pane is where the user command history gets listed
  This pane also shows other helpful Error / Log messages
 There is one textEntry box where commands are typed
 
 This program is solely to test and debug programs, it may never be 
 maintained for future or generalised. Not much time should be devoted to
 this.
 
 CURRENTLY
   Fully Functional!!!
   This should be used with ADCtest.c
   The raw int value is converted to volts in this program @lines 13x, 14x
 
 BUGS
   Terminal that is used to open this Program gets blocked even though the GUI
     window has been closed.
     - Thread for polling port does not properly terminate
      /*********************************************************\
     -|   Every connect-disconnect cycle spawns a new thread!!  |
      |   It is recomended that you kill the terminal, NOT JUST |
      |   THE GUI after a few cycles, say 3.                    |                      |
      \*********************************************************/
'''
import Tkinter as tk
import Queue, serial, threading
# Next line, is the serial port being used by the device
# It should be the same one as you see @ bottom right corner of ARDUINO IDE
# afer connecting the device
PORT = '/dev/ttyUSB0'

class Application(tk.Frame):
  def __init__(self, master=None):
    tk.Frame.__init__(self, master)
    self.grid()
    # the serial port
    self.ser = serial.Serial()
    # a status flag, denotes whether we have opened a serial port
    self.serUP = False
    self.q = Queue.Queue()
    self.io = ioThread(self.q, self.ser)
    self.createWidgets()
  
  def createWidgets(self):
    self.oFrame = tk.Frame(self)
    self.iFrame = tk.Frame(self)
    self.iflux = tk.Text(self.iFrame, height=24, padx=5, pady=5, relief=tk.RIDGE)
    self.oflux = tk.Text(self.oFrame, height=24, padx=5, pady=5, relief=tk.RIDGE, bg="pink")
    self.cmd = tk.Entry(self.oFrame, bg="cyan")
    self.start = tk.Button(self.iFrame, text='Connect', fg = "green", command=self.init_ser)
    self.end = tk.Button(self.iFrame, text='Disconnect', fg = "red", command=self.end_ser)
    self.quitButton = tk.Button(self.oFrame, text='Quit', fg = "magenta", command=self.quit)
    self.placeWidgets()
  
  def placeWidgets(self):
    self.iFrame.grid()    
    self.iflux.grid()
    self.oFrame.grid(row=0, column=1)    
    self.oflux.grid()    
    self.cmd.bind('<KeyPress-Return>', self.__parse)
    self.cmd.bind('<KeyPress-KP_Enter>', self.__parse)
    self.cmd.insert(0, '$ ')
    self.cmd.focus_force()
    self.cmd.grid(row=1, column=0, sticky=tk.W, padx=40)    
    self.start.grid()   
    self.end.grid(row=1, column=0, sticky=tk.E, padx=40)
    self.quitButton.grid(row=1, column=0, sticky=tk.E)
  
  def init_ser(self):
    try:
      self.ser.port = PORT
      self.ser.timeout = None
      self.ser.open()
      self.serUP = True
      try:
        #start thread to listen on port
        self.io.start()
        #start polling function
        self.processQ()
        self.oflux.insert('1.0', '*_* Opened '+PORT+', now listening for messages!\n')
      except:
        self.oflux.insert('1.0', '*_* Some thread error, restart propram!\n')
    except:
      self.oflux.insert('1.0', '*_* Error in setting up port @ ', PORT, '\n')
  
  def processQ(self):
    while self.q.qsize():
      try:
        self.iflux.insert('1.0', str(self.q.get(0))+'\n')
      except Queue.Empty:
        pass
    if self.serUP:
      self.after(10, self.processQ)

  def end_ser(self):
    if self.serUP:
      self.io.end()
      self.ser.flushInput()
      self.ser.flushOutput()
      self.ser.close()
      self.oflux.insert('1.0', '*_* Closed port: '+PORT+'\n')
      self.q.put('*_* Session closed normally --------------\n')
      self.serUP = False
  
  def __parse(self, event):
    cmd = self.cmd.get()
    if not cmd=='$ ':
      # log the command onto the pink pane
      self.oflux.insert('1.0', cmd+'\n')
    self.cmd.delete('2', tk.END)
    self.ser.write(cmd[2:])

class ioThread:
  def __init__(self, queue, ser):
    self.q = queue
    self.ser = ser
    self.running = False
  
  def start(self):
    self.running = True    
    self.influx = threading.Thread(target=self.listen)
    self.influx.start()    

  def end(self):
    self.running = False
  
  def listen(self):
    while self.running:
      data = self.ser.read()
      if not data == '':	#enter if-block only if some data has been sent to PC
        sz = ord(data)	#ord converts char to int
        info = 0		#actual data sent in packet (currently only working with ints)
        for i in range(0,sz):
          data = self.ser.read()
          info += ord(data)*(256**i)	#some trivial math
        self.q.put(info)
        '''
        if info>60000:
          self.q.put('* '+str(info))
        else:
          self.q.put(str(info))
        '''

app = Application()
app.master.title('T.A.L.K')
app.mainloop()

