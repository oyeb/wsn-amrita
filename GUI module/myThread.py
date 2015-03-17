import threading

class ioThread:
	def __init__(self, queue, ser):
		self.running = False
		self.q = queue
		self.ser = ser

	def start(self):
		self.running = True
		self.influx = threading.Thread(target=self.listen)
		self.influx.start()

	def end(self):
		self.running = False

	def listen(self):
		while self.running:
			data = self.ser.read()
			if not data == '':  #enter if-block only if some data has been sent to PC
				sz = ord(data)  #ord converts char to int
				info = 0    #actual data sent in packet (currently only working with ints)
				for i in range(0,sz):
					data = self.ser.read()
					info += ord(data)*(256**i)  #some trivial math
				self.q.put(info * 0.004882813)
"""
def show_snap(self):
	h = self.h
	m = self.m
	s = self.s - self.count
	if (s < 0):
			s += 60
			m -= 1
			if (m < 0):
				m = 60+m
				h -= 1
				if (h < 0):
					h = 24+h
	self.parent.oflux.insert('1.0', 'Downloaded %2d Logs. From %2d:%2d:%2d to %2d:%2d:%2d.' % (self.count, h, m, s, self.h, self.m, self.s))
	for i in range(0, self.count):
		s += 1
		if (s == 60):
				s = 0
				m += 1
				if (m == 60):
					m = 0
					h += 1
					if (h == 24):
						h = 0
		msg = '{:2d} {:2d} {:2d} {:1.5f}'.format(h, m, s, self.snap.pop(0))
		self.q.put(msg)
		self.parent.logfile.write(msg+'\n')
	self.q.put('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')

def listen(self):
	while self.running:
		data = self.ser.read()
		if not data == '':	#enter if-block only if some data has been sent to PC
			sz = ord(data)	  #ord converts char to int
			if sz==2:
				info = 0	      #actual data sent in packet (currently only working with ints)
				for i in range(0, 2):
					data = self.ser.read()
					info += ord(data)*(256**i)	#some trivial math
				self.snap.append(info * 0.004882813)
				self.count += 1
			else:
				#any size data-packet. Unload packet into a list of ints
				#save bytes into a list of ints. interpret ints as whatever u want
				info = []
				for i in range(0, sz):
					data = self.ser.read()
					info.append( ord(data) )
				self.h = info[0]
				self.m = info[1]
				self.s = info[2]
				self.show_snap()
				self.snap=[]
				self.count = 0
"""
