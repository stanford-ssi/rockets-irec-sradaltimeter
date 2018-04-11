import sys
import glob
import struct
import numpy
import serial
import csv
import defs
import pickle
from defs import *
FILE_PATH = './realdata/data.pickle'


formats = {FBMP:'ffff', FMMA:'ff',FBNO:'ffffffffffffffff',FGPS:'Qfffi',FVBAT:'f'}

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')
    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

#print(serial_ports())
#exit()

if __name__ == '__main__':


	#reading CSV file into memory, sorting into arrays for each sensr
	with open(FILE_PATH, 'rb') as f:
		data = pickle.load(f)
	if 0:
		sensor = FBMP
		time=0
		ind = data[sensor].index.get_loc(time,method='nearest')
		dat = [data[sensor][k].iloc[ind] for k in data[sensor].keys()]
		fetch = struct.pack(formats[sensor],*dat)
		print(':'.join([hex(k) for k in fetch]))
		exit()
		
	teensy = serial.Serial('/dev/ttyACM0')
	#wait for teensy configured for SITL testing
	while(1):
		read = teensy.read(1)
		print(read)
		if read == FSTART:
			print('>>> Skybass found ready for SITL testing')
			teensy.write(FSTART)
			break
	while(1):
		read = teensy.read(1) 

		if read == FSTART:
			request = teensy.read(5)
			sensor = request[0]
			#print(">>>Sensor Requested: ", sensor)
			time = struct.unpack('f',request[1:5])
			if sensor == FVBAT:
				fetch = struct.pack('f',3.8)
				teensy.write(fetch)
			else:
				ind = data[sensor].index.get_loc(time,method='nearest')
				dat = [data[sensor][k].iloc[ind] for k in data[sensor].keys()]
				fetch = struct.pack(formats[sensor],*dat)
				teensy.write(fetch)	

		else: 
			print(read.decode("utf-8"),end='')


        
        

    
    
    
    
    
    
    




