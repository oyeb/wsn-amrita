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
'''
import Tkinter as tk

class Application(tk.Frame):
  def __init__(self, master=None):
    tk.Frame.__init__(self, master)
    self.grid()
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
    print 'Initialising serial port!'

  def end_ser(self):
    print 'Closed port!!'
  
  def __parse(self, event):
    print 'Parse user command and sending to serial'

app = Application()
app.master.title('T.A.L.K')
app.mainloop()

