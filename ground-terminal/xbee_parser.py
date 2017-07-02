import sys
import glob
import time
import serial
import struct
import numpy as np
import csv
import matplotlib.pyplot as plt
RX_START = b'\x40'
RX_END   = b'\xB0'

now = time.strftime("%Y%m%d-%H%M")
BIN_NAME = "IREC_FLIGHT_" + now + ".bin"
CSV_NAME = "IREC_FLIGHT_" + now + ".log"

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
f_csv = open(CSV_NAME, "a")

print(f_csv)
fig = plt.plot()
while(1):
    #try:
        byte = xbee.read(1)
        if byte == RX_START:
            length_byte = xbee.read(1)
            length = struct.unpack('B', length_byte)[0]
            if length != 124:
                continue 
            message = xbee.read(length - 2)
            full_message = byte + length_byte + message
            #try:
                # static pressure in Pa 
            ind = 0
            time = struct.unpack('L', message[ind:(ind+4)])[0]
            ind += 4
            pressure1 = struct.unpack('f', message[ind:(ind+4)])[0] 
            ind += 4
            temp1 = struct.unpack('f', message[ind:(ind+4)])[0] 
            ind += 4
            pressure2 = struct.unpack('f', message[ind:(ind+4)])[0] 
            ind += 4
            temp2 = struct.unpack('f', message[ind:(ind+4)])[0] 
            ind += 4
            altitude = (1-(((pressure1 + pressure2)/2)/101350)**0.190284) * 145366.45
            temp = (temp1 + temp2)/2
            
            # acceleration in Gs
            mma_x = struct.unpack('f', message[ind:(ind+4)])[0]
            ind += 4
            mma_y = struct.unpack('f', message[ind:(ind+4)])[0]
            ind += 4

            bno_lina = struct.unpack('fff', message[ind:(ind+4*3)])
            ind += 4*3
            bno_quat = struct.unpack('ffff', message[ind:(ind+4*4)])
            ind += 4*4
            bno_euler = struct.unpack('fff', message[ind:(ind+4*3)])
            ind += 4*3
            bno_gyro = struct.unpack('fff', message[ind:(ind+4*3)])
            ind += 4*3
            bno_rota = struct.unpack('fff', message[ind:(ind+4*3)])
            ind += 4*3

            time_g = struct.unpack('Q',message[ind:(ind+8)])[0]
            ind += 8
            lon = struct.unpack('f',message[ind:(ind+4)])[0]
            ind += 4
            lat = struct.unpack('f',message[ind:(ind+4)])[0]
            ind += 4
            alt = struct.unpack('f',message[ind:(ind+4)])[0]
            ind += 4
            lock = struct.unpack('?',message[ind:(ind+1)])[0]
            ind += 1

            vbat = struct.unpack('f',message[ind:(ind+4)])[0]
            ind += 4

            c = 0
            for b in full_message[0:(len(full_message)-2)]:
                c = c ^ b
            checkpassed = (c == full_message[len(full_message)-2])
            if checkpassed:
                data_print = [time, round(temp,4), round(altitude,6), round(mma_x,4), round(mma_y,4), lat, lon, alt, time_g, lock, checkpassed]
                data_log = [time,vbat,altitude,pressure1,pressure2,temp1,temp2,mma_x,mma_y,bno_lina,bno_quat,bno_euler,bno_gyro,bno_rota,lat,lon,alt,time_g,lock,checkpassed]
                f_csv.write(str(data_log))
                f_csv.write("\n")
                f_csv.close()
                f_csv = open(CSV_NAME, "a")
                print("{: >10} {: >10} {: >10} {: >10} {: >10} {: >20} {: >20} {: >20} {: >5} {: >5} {: >5}".format(*data_print))s
            #except:
                #print("Error Parsing")

    #except:
        #print("Error reading stream")




