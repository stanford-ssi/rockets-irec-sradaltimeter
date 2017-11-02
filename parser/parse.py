import sys
import struct
import numpy as np
import array
import matplotlib.pyplot as plt
import csv

def get_string(buf, idx):
    i = 0
    s = ""
    while True:
        c = struct.unpack_from('c', buf, idx+i)[0]
        print(c)
        if c == b'\x00':
            break
        s += c.decode("utf-8")
        i += 1
    print(s)
    return s, i+1

f = open("data30.bin", "rb")

t = f.read()

sizes = {}
names = {}

unpacks = {'event': lambda x: struct.unpack('I', x)[0],
	'mma' : lambda x: struct.unpack('ff', x), 
	'bmp' : lambda x: struct.unpack('ffff', x), 
	'bno' : lambda x: struct.unpack('ffffffffffffffff', x),
	'gps' : lambda x: struct.unpack('Qfff????', x),
	'vbat': lambda x: struct.unpack('f', x)
	}
data = {'event': [], 'mma': [], 'bmp' : [], 'bno' : [], 'gps' : [], 'vbat' : []}
dts = {'event': [], 'mma': [], 'bmp' : [], 'bno' : [],'gps' : [], 'vbat' : []}

i = 0
nb = 0
ne = 0
while True:
    try:
        count = struct.unpack_from('H', t, i)[0]
    except:
        print("failed")
        break
    if count == 0: break
    nb += 1
    if i == 0:
        version, of = get_string(t, i+2)
        if count != 510 or version != ":skeleton:":
            print("Error parsing file - wrong format.")
            exit(1)
        idx = i+2+of
        while True:
            idd = struct.unpack_from('B', t, idx)[0]
            if idd == 255: break
            idx += 1
            name, of = get_string(t, idx)
            idx += of
            sz = struct.unpack_from('B', t, idx)[0]
            idx += 1
            sizes[idd] = sz
            names[idd] = name
    else:
        idx = i+2
        st = idx
        while True:
            if idx - st == 510: break
            idd = struct.unpack_from('B', t, idx)[0]
            if idd == 255: break
            ne += 1
            idx += 1
            time = struct.unpack_from('L', t, idx)[0]*10
            dts[names[idd]].append(time)
            idx += 4
            sz = sizes[idd]
            content = unpacks[names[idd]](t[idx:idx+sz])
            idx += sz
            data[names[idd]].append(content)
    i += 512
print("Parsed", nb, "blocks")
print("Total", ne, "entries")

print("Checking if data is not corrupted...", ("We're good" if data['event'] == range(len(data['event'])) else "Welp it got corrupted"))
print("Average times")
for k in dts:
    v = np.array(dts[k])*1e-6
    print(k, 1/np.diff(v).mean())

for key in data:
    print(key)
    dat = []
    i = 0
    for d in data[key]:
        row = []
        row.append(dts[key][i])
        for r in d:
            row.append(r)
        i += 1
        dat.append(row)
    
    with open(key + ".csv", 'w') as f:
        writer = csv.writer(f,lineterminator='\n')
        writer.writerows(dat)
    
    





