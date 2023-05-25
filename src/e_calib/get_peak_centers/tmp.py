#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type


import sys
import numpy as np


a0 = -3.03101e-06
a1 = -0.0773531
a2 = 0.612847

# B=100
x0 = 58
scale_f = 1
p0 = 1213.15

# B=101
#x0 = 56.28
#scale_f = 1.010
#p0 = 1225.19


# B=102
#x0 = 52.67
#scale_f = 1.021
#p0 = 1237.36

# B=98.5
#x0 = 66.18
#scale_f = 0.985
#p0 = 1194.88


def f(delta_x):
    return a0*delta_x*delta_x + a1*delta_x + a2


def two_body_kin(p):
    u = 931.49410242
    me = 0.511
    # projectile mass (MeV)
    m_b = 4*u+2424.91587/1000 - 2*me
    # ejectile mass (MeV)
    m_a = m_b
    # projectile kinematic energy (MeV)
    E_b = 206
    # target mass (MeV)
    m_Y = 24*u-13933.578/1000 - 12*me
    # residual mass (MeV)
    m_X = m_Y
    # ejectile angle (deg)
    theta = 4.5
    # ejectile kinematic energy (MeV)
    E_a = np.sqrt(p*p + m_b*m_b) - m_b

    theta = theta/180.*3.141593
    f_a = E_a*E_a + 2*E_a*m_a;
    f_b = E_b*E_b + 2*E_b*m_b;
    Ex = np.sqrt(np.power(E_b + m_b + m_Y - E_a - m_a, 2) - f_a - f_b +
            2*np.cos(theta)*np.sqrt(f_a*f_b)) - m_X;

    return Ex


if len(sys.argv) != 2:
    print('usage: ./xxx <x position (mm)>')
    sys.exit()

x = float(sys.argv[1])
delta_x = x - x0
if delta_x > 110 or delta_x < -385:
    print('warning: delta x outside range [-385mm, 110mm], extrapolation will be used!')

delta_p = f(delta_x)
delta_p *= scale_f
p = p0 + delta_p 
Ex = two_body_kin(p)

print('focal plane position: x=%.1f(mm), excitation energy: %.3f(MeV)' %
        (x, Ex))



