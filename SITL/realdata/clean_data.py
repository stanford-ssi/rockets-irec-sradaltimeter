#Cleans up 
#
#

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import csv 
import sys
sys.path.append('../')
import defs
OLD_DIRNAME = 'SA_2017-6-22_Skybass/'
NEW_DIRNAME = 'SA_2017-6-22_Skybass_Clean/'

START_INDEX = 500000
#START_TIME = 1.06319e11
START_TIME = 106319000000 - 1e9 +9e8
bno_header = 'time,a_x,a_y,a_z,q_w,q_x,q_y,q_z,th_x,th_y,th_z,w_x,w_y,w_z,alph_x,alph_y,alph_z'
bmp_header = 'time,p1,T1,p2,T2'
mma_header = 'time,mma_x,mma_y'
gps_header = 'time,gps_time,gps_lon,gps_lat,gps_alt,gps_lock'

# plot accel data and use that to to find a start time
def veiw_raw():
	bno_list = []
	with open(OLD_DIRNAME + 'bno.csv', 'r') as csvfile:
		csvreader = csv.reader(csvfile, delimiter=',')
		last_t = 0
		time_accumilated = 0
		for row in csvreader:
			time = int(row[0])
			if time < last_t:
				time_accumilated += last_t
			row[0] = str(time + time_accumilated)
			bno_list.append([float(val) for val in row])
			last_t = time


	bno_data = np.array(bno_list)
	#plt.subplot(311)
	#plt.plot(bno_data[:,0])
	#plt.subplot(312)
	#plt.plot(bno_data[:,2])
	#plt.subplot(313)
	#plt.plot(bno_data[:,3])
	#plt.show()

def cleanfile(file, header):
	with open(NEW_DIRNAME + file + '.csv', 'w+') as wfile:
		with open(OLD_DIRNAME + file + '.csv', 'r') as rfile: 
			reader = csv.reader(rfile, delimiter=',')
			writer = csv.writer(wfile, delimiter=',',quoting=0)
			wfile.write(header+ '\n')
			i = 0
			time_accumilated = 0
			last_t = 0
			for row in reader:
				time = int(row[0])
				if time<last_t:
					time_accumilated += last_t
				if (time+time_accumilated > START_TIME):
					row[0] = str(time+time_accumilated)
					if file == 'gps':
						row = row[:-3]
						row[-1] = '1' if row[-1] == 'True' else '0'
					writer.writerow(row)
				i += 1
				last_t = time

def clean_data():
	cleanfile('bno',bno_header)
	cleanfile('mma',mma_header)
	cleanfile('bmp',bmp_header)
	cleanfile('gps',gps_header)

def make_dataframe():
	import pickle
	bno_data = pd.DataFrame.from_csv(NEW_DIRNAME+'bno.csv')
	mma_data = pd.DataFrame.from_csv(NEW_DIRNAME+'mma.csv')
	bmp_data = pd.DataFrame.from_csv(NEW_DIRNAME+'bmp.csv')
	gps_data = pd.DataFrame.from_csv(NEW_DIRNAME+'gps.csv')
	gps_data.gps_time.astype(int)
	gps_data.gps_lock.astype(int)
	data = {}
	data[defs.FBNO] = bno_data	
	data[defs.FMMA] = mma_data	
	data[defs.FBMP] = bmp_data	
	data[defs.FGPS] = gps_data	
	raw_dat = pd.concat([bno_data, mma_data, bmp_data, gps_data],axis=1)
	tmin = min(raw_dat.index)
	for key,sensor in data.items():
		sensor.index = (sensor.index - tmin)/10
		#print(sensor.index)
	with open('data.pickle','wb') as f:
		pickle.dump(data,f)

	raw_dat = pd.concat([bno_data, mma_data, bmp_data, gps_data],axis=1)
	raw_dat.index = (raw_dat.index - min(raw_dat.index))
	raw_dat.p1.dropna().plot()
	plt.show()
clean_data()
make_dataframe()