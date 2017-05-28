import csv
import matplotlib.pyplot as plt
import numpy as np 
from numpy import genfromtxt
import math

bmp_array = genfromtxt('bmp.csv', delimiter=',')
mma_array = genfromtxt('mma.csv', delimiter=',')
start = 713000
end =  720000


pressure = (bmp_array[start:end,1] + bmp_array[start:end,2]) / 2
altitude = (1-((pressure)/101350)**0.190284) * 145366.45
ax = mma_array[start:end,1]
ay = np.copy(mma_array[start:end,2])
plt.plot(ay)
#plt.plot(altitude)
plt.show()

