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
   Only listens on port, does not send any data.
   GUI freezes upon pressing 'Connect' as a busy-while loop vlocks everything
   Must handover that task to a thread...
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
      print '*_* Opened', PORT, 'listening for messages!'
      while True:
        data = self.ser.read()
        if not data == '':	#enter if-block only if some data has been sent to PC
          sz = ord(data)	#ord converts char to int
          info = 0		#actual data sent in packet (currently only working with ints)
          for i in range(0,sz):
            data = self.ser.read()
            info += ord(data)*(256**i)	#some trivial math
          if info>60000:
            print '* ', info
          else:
            print info
    except:
      print '*_* Error in setting up port @ ', PORT

  def end_ser(self):
    if self.serUP:
      self.ser.close()
      self.serUP = False
  
  def __parse(self, event):
    cmd = self.cmd.get()
    if not cmd=='$ ':
      # log the command onto the pink pane
      self.oflux.insert('1.0', cmd+'\n')
    self.cmd.delete('2', tk.END)

app = Application()
app.master.title('T.A.L.K')
app.mainloop()

