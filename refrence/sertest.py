# This script flushes every byte received on the serial port

import serial
import sys
import numpy as np
import time

num_samples = 700000
index = 0

ser = serial.Serial(
   port = 'COM22',
   baudrate = 1000000,
   parity = serial.PARITY_NONE,
   stopbits = serial.STOPBITS_ONE,
   bytesize = serial.EIGHTBITS
)

if ser.isOpen() == False :
   ser.open()
   if ser.isOpen() == False :
      print("Can't open serial port")
      sys.exit(1)

data_array = np.empty(num_samples, dtype=np.int16)

while 1 :
   start = time.time()

   while index < num_samples:
      if ser.inWaiting() > 0 :
         data = ser.readline().decode().strip()
         if data:
            try:
               data_array[index] = data
               index += 1
               if index == 300:
                  print(data)
            except ValueError:
               print("Invalid dara: ", data)

   end = time.time()
   print("Receied ", num_samples, " items")
   print("Took ", end - start, "seconds")
   index = 0