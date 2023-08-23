import sys
import serial
import numpy as np
import time

num_samples = 700000;

ser = serial.Serial('COM22', 2000000)

if ser.isOpen() == False :
    ser.open()
    if ser.isOpen() == False :
        print("Tried to open serial port, but failed")
        sys.exit(1)

count = 0
int_received = 0
data_array = np.empty(num_samples, dtype=np.int32)

while True :
    start = time.time()

    while int_received < num_samples:
        data = ser.read(2)

        if len(data) == 2:
            try:
                data_array[int_received] = int.from_bytes(data, byteorder='little', signed=True)
                int_received += 1
                if int_received == 900:
                    print(data)
            except ValueError:
                print("Invalid Data Received: ", data)

    end = time.time()
    elapsed = end - start
    print("Received ", data_array.size, " samples")
    print("Took ", elapsed, "seconds")
    #count += 1
    int_received = 0;
    #if count == 3:
        #np.savetxt("samples", data_array, delimiter=',')





