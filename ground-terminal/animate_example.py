# Example 1: decay
# http://matplotlib.org/examples/animation/animate_decay.html
# This example showcases a sinusoidal decay animation.

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation


def data_gen(t=0):
    cnt = 0
    while cnt < 1000:
        cnt += 1
        t += 0.1
        yield t, np.sin(2*np.pi*t) * np.exp(-t/10.)


def init():
    ax.set_ylim(-1.1, 1.1)
    ax.set_xlim(0, 10)
    del xdata[:]
    del ydata[:]
    line.set_data(xdata, ydata)
    return line,

fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.grid()
xdata, ydata = [], []


def run(data):
    # update the data
    t, y = data
    xdata.append(t)
    ydata.append(y)
    xmin, xmax = ax.get_xlim()

    if t >= xmax:
        ax.set_xlim(xmin, 2*xmax)
        ax.figure.canvas.draw()
    line.set_data(xdata, ydata)

    return line,

ani = animation.FuncAnimation(fig, run, data_gen, blit=False, interval=10,
                              repeat=False, init_func=init)
plt.show()

# %% example two
# http://scipy-cookbook.readthedocs.io/items/Matplotlib_Animations.html

from pylab import *
import time

ion()

tstart = time.time()               # for profiling
x = arange(0,10,1)            # x-array
line, = plot(x,sin(x))
for i in arange(1,10):
    # line.set_ydata(input("num please"))
    line.set_ydata(sin(x+i/10.0))  # update the data
    draw()                         # redraw the canvas

print ('FPS: ',200/(time.time()-tstart))

# %% example three 
# http://scipy-cookbook.readthedocs.io/items/Matplotlib_Animations.html
# uses gtk (which i can't seem to find for python 3 rn)

import sys
import gtk, gobject
import pylab as p
import matplotlib.numerix as nx
import time

ax = p.subplot(111)
canvas = ax.figure.canvas

# for profiling
tstart = time.time()

# create the initial line
x = nx.arange(0,2*nx.pi,0.01)
line, = p.plot(x, nx.sin(x), animated=True)

# save the clean slate background -- everything but the animated line
# is drawn and saved in the pixel buffer background
background = canvas.copy_from_bbox(ax.bbox)

def update_line(*args):
    # restore the clean slate background
    canvas.restore_region(background)
    # update the data
    line.set_ydata(nx.sin(x+update_line.cnt/10.0))
    # just draw the animated artist
    ax.draw_artist(line)
    # just redraw the axes rectangle
    canvas.blit(ax.bbox)

    if update_line.cnt==50:
        # print the timing info and quit
        print('FPS: ' , update_line.cnt/(time.time()-tstart))
        sys.exit()

    update_line.cnt += 1
    return True
update_line.cnt = 0

gobject.idle_add(update_line)
p.show()

# %% https://jakevdp.github.io/blog/2012/08/18/matplotlib-animation-tutorial/

"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

# First set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
line, = ax.plot([], [], lw=2)

# initialization function: plot the background of each frame
def init():
    line.set_data([], [])
    return line,

# animation function.  This is called sequentially
def animate(i):
    x = np.linspace(0, 2, 1000)
    y = np.sin(2 * np.pi * (x - 0.01 * i))
    line.set_data(x, y)
    return line,

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=200, interval=20, blit=True)

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html

plt.show()


