import serial
import time
import struct
import pickle

ser = serial.Serial(port='COM22', baudrate=1000000)
SAMPLES = 30000
s = struct.Struct('<' + str(SAMPLES) + 'H')  # struct for binary float data

ser.reset_input_buffer()
start = time.time()
ser.write("r".encode())  # just a char, doesn't matter which char
serial_data = ser.read(SAMPLES * 2)  # float is 4 bytes
unpacked_data = s.unpack(serial_data)
print(f"took {time.time() - start}")
print('Unpacked Values:', unpacked_data[0:20])
print('Unpacked Values:', unpacked_data[29980:30000])

ser.close()