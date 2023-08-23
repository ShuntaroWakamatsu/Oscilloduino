# This script flushes every byte received on the serial port

import serial
import sys

ser = serial.Serial(
   port = 'COM19',
   baudrate = 115200,
   parity = serial.PARITY_NONE,
   stopbits = serial.STOPBITS_ONE,
   bytesize = serial.EIGHTBITS
)

if ser.isOpen() == False :
   ser.open()
   if ser.isOpen() == False :
      print("Can't open serial port")
      sys.exit(1)

while 1 :
   if ser.inWaiting() > 0 :
      ser.read(16384);