# xbee_plotter
# Ian Gomez, 06/13/17

import csv
import matplotlib.pyplot as plt
from IPython import get_ipython
import time

ipython = get_ipython()
backends = ['%matplotlib','%matplotlib inline']
ipython.magic(backends[0])

'''
 data = [time, round(altitude,6), round(pressure1), round(pressure2), 
        round(mma_x,4), round(mma_y,4), lat, lon, alt, time_g, lock, checkpassed]
 '''
 
def unpack_data(data):
    first = data[0][1:]
    last  = data[len(data)-1][:-1]
    return first, data[1], data[2], data[3], data[4], data[5], data[6], \
        data[7], data[8], data[9], data[10], last
 
f = open("FARFlight1.log","r")
reader = csv.reader(f, delimiter=',')

fig = plt.figure(1)
plt.xlabel('t (s)'); plt.ylabel('y (m)'); plt.title('Altitude'); plt.grid();
plt.show();  #yy = fig.add_subplot(111)
#line1, = yy.plot(t, alt, 'b', label='y(t)')  
plt.scatter(5,5)
plt.scatter(7,7)
plt.show


#for row in reader:
#    time.sleep(1)
#    t, alt, P1, P2, ax, ay, gps_lat, gps_lon, gps_alt, gps_t, gps_lock, checksum = unpack_data(row)
#    print('t: {}, alt: {}'.format(t, alt))
#    plt.scatter(t,alt)
#    plt.show