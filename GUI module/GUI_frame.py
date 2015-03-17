import Tkinter as tk
import Queue

class Application(tk.Frame):

	def __init__(self, master = None):
		tk.Frame.__init__(self, master)
		self.q = Queue.Queue()
		self.serUP = False
		self.grid()
		self.createWidgets()

	def createWidgets(self):
		self.oFrame = tk.Frame(self)
		self.iFrame = tk.Frame(self)
		self.iflux = tk.Text(self.iFrame, height=24, padx=5, pady=5, relief=tk.RIDGE)
		self.oflux = tk.Text(self.oFrame, height=24, padx=5, pady=5, relief=tk.RIDGE, bg="pink")
		self.cmd = tk.Entry(self.oFrame, bg="cyan")
		self.quitButton = tk.Button(self.oFrame, text='Quit', fg = "magenta", command = self.quit)
		self.placeWidgets()

	def placeWidgets(self):
		self.iFrame.grid()
		self.iflux.grid()
		self.oFrame.grid(row=0, column=1)
		self.oflux.grid()
		self.cmd.insert(0, '$ ')
		self.cmd.focus_force()
		self.cmd.grid(row=1, column=0, sticky=tk.W, padx=40)
		self.quitButton.grid(row=1, column=0, sticky=tk.E)

	def setup(self, con, discon, parse):
		self.start = tk.Button(self.iFrame, text='Connect', fg = "green", command=con)
		self.end = tk.Button(self.iFrame, text='Disconnect', fg = "red", command=discon)
		self.start.grid()
		self.end.grid(row=1, column=0, sticky=tk.E, padx=40)
		self.cmd.bind('<KeyPress-Return>', parse)
		self.cmd.bind('<KeyPress-KP_Enter>', parse)

	def right_put(self, msg):
		self.oflux.insert('1.0', msg+'\n')

	def left_put(self, msg):
		self.iflux.insert('1.0', msg+'\n')

	def processQ(self):
		while self.q.qsize() <> 0:
			try:
				self.iflux.insert('1.0', str(self.q.get(0))+'\n')
			except Queue.Empty:
				pass
		if self.serUP:
			self.after(10, self.processQ)
