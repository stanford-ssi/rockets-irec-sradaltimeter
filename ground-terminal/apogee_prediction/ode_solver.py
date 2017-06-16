# ODE Solver to shoot for final apogee solutions
# Ian Gomez, May 21
# Currently assumes: 1D, no lift, constant mass (after burnout)

import csv
import time
import numpy as np
from scipy.integrate import odeint
start_time = time.time()

# some constants
g = 9.81                    # m/s^2
S = np.pi * 0.1016**2 / 4   # m^2
m = 18.1437                 # kg
launch_elevation = 1293     # m
Tlocal = 288                # K
k = 1.4                     # air specific heat ratio
R_air = 287                 # kJ/kg
local_c = np.sqrt(k*R_air*Tlocal)

# read in CD at aoa = 0 data
cdata = open('CD0.csv',"r")
reader = csv.reader(cdata)
rownum, colnum = 0, 0
w, h = 2, 2500
CD0vec = [[0 for x in range(w)] for y in range(h)]
for row in reader:
    colnum = 0
    for col in row:
        CD0vec[rownum][colnum] = float(col)
        colnum += 1
    rownum += 1

# function which determines acceleration
def rocket(ry, t):
    y, uy = ry
    drydt = [uy, get_fy(y,uy)/m]
    return drydt

# takes in altitude to return approximate density
def get_rho(h):
    rho_b = 1.225   # density at sea level
    R   = 8.3145    # universal gas constant
    g0  = 9.80665   # local gravity
    M   = 0.0289644 # molar mass of air
    T_b = 288       # temperature of air at sea level
    rho = rho_b**(-g0 * M * h / R / T_b) # final density 
    return rho

# solves for forces in y direction
def get_fy(y, uy):
    gravity = g*m
    rho = get_rho(y+launch_elevation)
    drag = 0.5*get_CD(uy)*S*rho*uy**2
    fy = -drag + -gravity
    return fy

# pull CD from table
def get_CD(uy):
    mach = uy/local_c;
    k = 1; tol = 0.01;
    while(1):
        delta = np.abs(mach-CD0vec[k][0]);
        if delta < tol:
            break
        k = k + 1;
    CD = CD0vec[k][1]
    return CD

# solver
def shoot(ry0, t0, tspan): 
    t = np.linspace(t0, tspan, 10001)    # set up time vector
    sol = odeint(rocket, ry0, t)         # solve
    return t, sol[:, 0]

print("--- %s seconds ---" % (time.time() - start_time))