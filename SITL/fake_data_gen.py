import os
import random
import math 

n_sensors = 5

def random_gen():
	try:
		os.remove("fakedata/fake1.csv")
	except:
		pass
	file = open("fakedata/fake1.csv", "w+")
	for i in range(0,10001):
		if math.floor(random.random()*n_sensors) == 0:
			file.write('1' + ',' +  str(i*1000.0) + ',' + str(random.random()*100) + ',' + str(random.random()*100)+'\n')

		if math.floor(random.random()*n_sensors) == 1:
			file.write('2' + ',' +  str(i*1000.0) + ','  + str(random.random()*100)+'\n')
			
		if math.floor(random.random()*n_sensors) == 2:
			file.write('3' + ',' +  str(i*1000.0) + ',' + str(random.random()*100) + ',' + str(random.random()*100)+'\n')

		if math.floor(random.random()*n_sensors) == 3:
			file.write('4' + ',' +  str(i*1000.0) + ',' + str(random.random()*100) + ',' + str(random.random()*100) + ',' + str(random.random()*100)+'\n')


def random_gen():

random_gen()	