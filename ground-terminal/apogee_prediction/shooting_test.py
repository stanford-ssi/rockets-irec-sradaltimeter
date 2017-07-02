import ode_solver
import time
import numpy as np
import matplotlib.pyplot as plt
from IPython import get_ipython
ipython = get_ipython()
backends = ['%matplotlib','%matplotlib inline']
ipython.magic(backends[0])

start_time = time.time()

m2kft = 3.28084/1e3            # kft/m

ry0 = [2253.996, 575]               # initial conditions
sol = ode_solver.shoot(ry0, 0, 20)

t = sol[0]; y = sol[1]*m2kft
fig = plt.figure(1)
plt.xlabel('t (s)'); plt.ylabel('y (kft)'); plt.title('Altitude'); plt.grid();
yy = fig.add_subplot(111); line1, = yy.plot(t, y, 'b', label='y(t)') 
plt.show()
 
# %% 
plt.pause(0.5)
ry1 = [4572, 575-100]               # initial conditions
sol1 = ode_solver.shoot(ry1, 5, 20)
t1 = sol1[0]; y1 = sol1[1]*m2kft
plt.plot(t1,y1,'r')

#line1.set_ydata(y1)
#fig.canvas.draw()
