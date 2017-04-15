import sys
import glob
import serial
import struct
import numpy
import csv

RX_START = b'\x40'
RX_END   = b'\xB0'


with open("20170411 IREC Avionics Skyline Radio Test.log", "rb") as f:
    byte = f.read(1)
    while byte != "":
        if byte == RX_START:
            length = f.read(1)
            message = byte + length + f.read(struct.unpack('B', length)[0] - 3)
            #print(message)

            # static pressure in Pa 
            time = struct.unpack('L', message[0:4])[0] 
            pressure1 = struct.unpack('f', message[4:8])[0]   
            pressure2 = struct.unpack('f', message[8:12])[0]  
        
            # acceleration in Gs
            mma_x = struct.unpack('f', message[12:16])[0]
            mma_y = struct.unpack('f', message[16:20])[0]

            bno_x = struct.unpack('f', message[20:24])[0]
            bno_y = struct.unpack('f', message[24:28])[0]
            bno_z = struct.unpack('f', message[28:32])[0]
            time = struct.unpack('Q',message[32:40])[0]
            lat = struct.unpack('Q',message[40:44])[0]
            lon = struct.unpack('Q',message[44:48])[0]
            alt = struct.unpack('Q',message[48:52])[0]
            c = 0
            for b in message[0:(len(message)-2)]:
                c = c ^ b
                print(b)
            print("checksum:",message[len(message)-2])
            print("checksum:",c) 
            print("endmessage")

        byte = f.read(1)

