"""
Send a start and a stop signal to arduino over serial port
then reads sound data which the arduino sends over the serial port.
Then sends this data to Microsoft Psi
NOTE: You may need to change the serial port
"""

__author__ = "Sander Schulhoff"
__email__ = "Sanderschulhoff@gmail.com"

port = 'COM3'

# imports
import serial
import time
import zmq
import msgpack
from datetime import datetime
from time import sleep
import matplotlib.pyplot as plt

# open port connection
ser = serial.Serial(port=port, timeout=1)

# array for sound data
data_array = []

# flush IO just in case
ser.flushOutput()
ser.flushInput()

# wait a second
time.sleep(1)

# send the number 1 to indicate to arduino it should start recording
ser.write(bytes("1", 'utf-8'))

time.sleep(10)

# make arduino stop recording
ser.write(bytes("2", 'utf-8'))

print("start reading")

while(True):
    # will read empty and generate error after last number received
    data = ser.readline()
    try:
        # decode the bytes and append the data sample to the array
        decoded_bytes = float(data[0:len(data)-2].decode("utf-8"))
        data_array.append(decoded_bytes)
    except:
        break

print("done reading")

# send each data sample over tcp to Microsoft Psi
output = zmq.Context().socket(zmq.PUB)
output.bind("tcp://127.0.0.1:30001")

# sends one integer value every second to demonstrate how Psi works with data streams
# can be easily changed to send full data array 
for index, data in enumerate(data_array):
    payload = {}
    payload[u"message"] = data
    # payload must have originatingTime
    payload[u"originatingTime"] = datetime.now().timestamp()
    sleep(1)
    # copied from Psi docs/examples
    output.send_multipart(["sound".encode(), msgpack.dumps(payload)])
    