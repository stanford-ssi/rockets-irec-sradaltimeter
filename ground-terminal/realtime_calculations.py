# Ian Gomez

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# fake data 
v_x = np.array([2, 3, 5, 10])    # m/s
v_y = np.array([0, 100, 243, 400])  # m/s
h   = np.array([0, 100, 500, 2000])# m 

# takes in altitude to return approximate density
def get_rho(h):
    rho_b = 1.225   # density at sea level
    R   = 8.3145    # universal gas constant
    g0  = 9.80665   # local gravity
    M   = 0.0289644 # molar mass of air
    T_b = 288       # temperature of air at sea level
    rho = rho_b**(-g0 * M * h / R / T_b) # final density 
    return rho
    
# dynamic pressure (helps find max q)
def get_q(v_x, v_y, h):
    v_norm = np.sqrt(v_x**2 + v_y**2)
    q = 0.5 * get_rho(h) * v_norm**2
    return q

rho = get_rho(h)
q = get_q(v_x, v_y, h)


print('q: {}\nrho: {}'.format(q, rho))
