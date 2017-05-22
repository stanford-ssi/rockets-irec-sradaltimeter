import sys
import glob
import serial
import struct
import numpy as np
import csv

RX_START = b'\x40'
RX_END   = b'\xB0'

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

print(serial_ports())
xbee = serial.Serial('COM4',115200)
while(1):
    byte = xbee.read(1)
    if byte == RX_START:
        length = xbee.read(1)
        message = xbee.read(struct.unpack('B', length)[0] - 2)
        #print(message)

        # static pressure in Pa 
        time = struct.unpack('L', message[0:4])[0] * 10^(-6)
        pressure1 = struct.unpack('f', message[4:8])[0]   
        pressure2 = struct.unpack('f', message[8:12])[0]  
        
        # acceleration in Gs
        mma_x = struct.unpack('f', message[12:16])[0]
        mma_y = struct.unpack('f', message[16:20])[0]

        bno_x = struct.unpack('f', message[20:24])[0]
        bno_y = struct.unpack('f', message[24:28])[0]
        bno_z = struct.unpack('f', message[28:32])[0]

        
        print(time,pressure1,pressure2,mma_x,mma_y,bno_x,bno_y,bno_z)
