import threading

class ioThread:
  def __init__(self, queue, ser, parent):
    self.running = False
    self.q = queue
    self.ser = ser
    self.count = 0
    self.snap = []
    self.parent = parent
    self.lfile = open('logs2', 'a')

  def start(self):
    self.running = True
    self.influx = threading.Thread(target=self.listen)
    self.influx.start()

  def end(self):
    self.running = False
    self.lfile.close()

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
      datum = self.snap.pop(0)
      gui_msg = '{:2d} {:2d} {:2d} {:1.5f}'.format(h, m, s, datum)
      file_msg = '{:d}:{:d}:{:d} {:1.5f}'.format(h, m, s, datum)
      self.lfile.write(file_msg+'\n')
      self.q.put(gui_msg)
    self.q.put('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')

  def listen(self):
    while self.running:
      data = self.ser.read(1)
      if not data == '':  #enter if-block only if some data has been sent to PC
        sz = ord(data)    #ord converts char to int
        if sz==2:
          info = 0        #actual data sent in packet (currently only working with ints)
          for i in range(0, 2):
            data = self.ser.read()
            info += ord(data)*(256**i)  #some trivial math
            self.snap.append(info * 0.004882813)
          self.count += 1
        else:
        #any size data-packet. Unload packet into a list of ints
        #save bytes into a list called info. interpret these bytes as whatever u want
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
