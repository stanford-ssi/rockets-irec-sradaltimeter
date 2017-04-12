# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 18:49:01 2017

@author: ME-123-10
"""

# %% example two
# http://scipy-cookbook.readthedocs.io/items/Matplotlib_Animations.html

from pylab import *
import time

ion()

tstart = time.time()               # for profiling
x = arange(0,10,.1)            # x-array
line, = plot(x,sin(x))
for i in arange(1,100):
    #line.set_ydata(input("num please"))
    line.set_ydata(sin(x+i/10.0))  # update the data
    draw()                         # redraw the canvas

