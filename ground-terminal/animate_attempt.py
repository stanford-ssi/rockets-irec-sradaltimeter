# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 18:49:01 2017

@author: ME-123-10
"""

# %% example two
# http://scipy-cookbook.readthedocs.io/items/Matplotlib_Animations.html

#from pylab import *
#from IPython import get_ipython
#ipython = get_ipython()
#backends = ['%matplotlib','%matplotlib inline']
#ipython.magic(backends[0])
#
#
##x = arange(0,10,.1)                # x-array
##line, = plot(x,sin(x))
##for i in arange(1,100):
##    #line.set_ydata(input("num please"))
##    line.set_ydata(sin(x+i/10.0))  # update the data
##    draw()                         # redraw the canvas
#
#x = arange(0,10,0.1)
#y = x**2
#plot(x,y)
#

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
    plt.pause(0.1)
    line1.set_ydata(np.sin(x + phase))
    fig.canvas.draw()
    
# %%

#import matplotlib.pyplot as plt
#import matplotlib.animation as anim
#import random
#from IPython import get_ipython
#ipython = get_ipython()
#backends = ['%matplotlib','%matplotlib inline']
#ipython.magic(backends[0])
#
#def plot_cont(fun, xmax):
#    y = []
#    fig = plt.figure()
#    ax = fig.add_subplot(1,1,1)
#
#    def update(i):
#        yi = fun()
#        y.append(yi)
#        x = range(len(y))
#        ax.clear()
#        ax.plot(x, y)
#        print(i, ': ', yi)
#
#    a = anim.FuncAnimation(fig, update, frames=xmax, repeat=False)
#    plt.show()
#
#def fun():
#    random.randint(0, 20)
#    
#plot_cont(fun, 10)