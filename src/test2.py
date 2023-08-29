import sys
import serial
import numpy as np
import struct
import time

num_samples = 100000;

ser = serial.Serial('COM22', 480000000)

s = struct.Struct('>' + str(num_samples) + 'H')

if ser.isOpen() == False :
    ser.open()
    if ser.isOpen() == False :
        print("Tried to open serial port, but failed")
        sys.exit(1)

count = 0
int_received = 0
data_array = np.empty(num_samples, dtype=np.int32)

ser.reset_input_buffer()

while True :
    start = time.time()

    serial_data = ser.read(num_samples * 2)
    print("Haha", serial_data[0:20])
    unpacked_data = s.unpack(serial_data)

    end = time.time()
    elapsed = end - start
    print('Unpacked Values:', unpacked_data[0:10])
    print("Took ", elapsed, "seconds")
    #count += 1
    int_received = 0;
    #if count == 3:
        #np.savetxt("samples", data_array, delimiter=',')





