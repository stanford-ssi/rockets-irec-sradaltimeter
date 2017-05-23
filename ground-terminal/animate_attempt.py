# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 18:49:01 2017

@author: ME-123-10
"""

# %%
from IPython import get_ipython
ipython = get_ipython()
backends = ['%matplotlib','%matplotlib inline']
ipython.magic(backends[0])
import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 6*np.pi, 100)
y = np.sin(x)

# You probably won't need this if you're embedding things in a tkinter plot...
plt.ion()

fig = plt.figure(1)
ax = fig.add_subplot(111)
line1, = ax.plot(x, y, 'r-') # Returns a tuple of line objects, thus the comma
plt.show()

for phase in np.linspace(0, 10*np.pi, 500):
    plt.pause(0.01)
    line1.set_ydata(np.sin(x + phase))
    fig.canvas.draw()
    