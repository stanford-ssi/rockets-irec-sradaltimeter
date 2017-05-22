import numpy as np
import matplotlib.pyplot as plt
from IPython import get_ipython
ipython = get_ipython()
backends = ['%matplotlib','%matplotlib inline']
ipython.magic(backends[0])


"ASSUME mass is constant"
ROCKET_MASS = 5
TIME_STEP = .01

state = 50, 5 # Initial position and velocity 
def F(x, v, t):
    drag = 0.5*0.75*1.225*v**2*(np.pi*(0.1016**2)/4)
    gravity = ROCKET_MASS*9.81
    force = drag + gravity
    return force

def rk4(x, v, F):
    """Returns final (position, velocity) tuple after
    time dt has passed.

    x: initial position (number-like object)
    v: initial velocity (number-like object)
    a: acceleration function a(x,v,dt) (must be callable)"""
    x1 = x
    v1 = v
    a1 = F(x1, v1, 0)*1.0 / (ROCKET_MASS)
    
    force = F(x1, v1, 0)*1.0 / (ROCKET_MASS)

    x2 = x + 0.5*v1*TIME_STEP
    v2 = v + 0.5*a1*TIME_STEP
    a2 = F(x2, v2, TIME_STEP/2.0) / (ROCKET_MASS)

    x3 = x + 0.5*v2*TIME_STEP
    v3 = v + 0.5*a2*TIME_STEP
    a3 = F(x3, v3, TIME_STEP/2.0) / (ROCKET_MASS)

    x4 = x + v3*TIME_STEP
    v4 = v + a3*TIME_STEP
    a4 = F(x4, v4, TIME_STEP) / (ROCKET_MASS)

    xf = x + (TIME_STEP/6.0)*(v1 + 2*v2 + 2*v3 + v4)
    vf = v + (TIME_STEP/6.0)*(a1 + 2*a2 + 2*a3 + a4)

    return xf, vf, a1, force

#print("Initial -position: %6.2f, velocity: %6.2f"%state)

t = 0; i = 0; ti = []; r = []; u = []; a = []
# Run for 100 seconds
while t < 100:
    t += TIME_STEP
    ti.append(t) 
    r.append(rk4(state[0], state[1], F)[0])
    u.append(rk4(state[0], state[1], F)[1])
    a.append(rk4(state[0], state[1], F)[2])

#    print(" -position: %6.2f, velocity: %6.2f"%state)
    i = i+1

fig1 = plt.figure(1) 
plt.plot(ti,r); plt.title('position')
fig2 = plt.figure(2)
plt.plot(ti,u); plt.title('velocity')
fig3 = plt.figure(3)
plt.plot(ti,a); plt.title('acceleration')
