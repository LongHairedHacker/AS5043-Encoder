#! /usr/bin/env python


import serial
from time import sleep
from datetime import datetime

import matplotlib.pyplot as plt



MAX_GRAD = 900

def read_angle():
	line = ""
	while len(line) == 0:
		ser.write("a")
		line = ser.readline()
	return int(line[:-2])



ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.032)
if not ser:
	print "Unable to open serial port"
	sys.exit(1)

values = []
samples = []
sample = 0

starttime = datetime.now()
for x in range(0,1000):
	ang = read_angle();
	values.append(ang)
	samples.append(sample)
	sample += 1

print "Done"
plt.plot(samples,values,'+',samples,values)
plt.show()


grads = []
for x in range(1,1000):
	cur_grad = values[x] - values[x-1];
	if(abs(cur_grad) < MAX_GRAD):
		grads.append(cur_grad)


plt.hist(grads,len(grads))
plt.show()

""""
avg_grad = 0
for grad in grads:
	avg_grad += grad / len(grads)

print "Avarage gradient: %f" % avg_grad

avg_grad_error = 0
for grad in grads:
	avg_grad_error += abs(grad - avg_grad) / len(grads)

print "Avarage gradient error: %f" % avg_grad_error
"""