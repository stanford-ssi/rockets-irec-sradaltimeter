"""
A simple example of an animated plot
"""
import numpy as np
import os


f = open("FAKE_FLIGHT.log", "a", os.OS_NONBLOCK)
print(f)

while 1:
	f.write("test")
	print("test")
	f.flush()
	pause(1);
