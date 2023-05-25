#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type

import numpy as np
import sys


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
E_a = 200.19


if len(sys.argv) == 2:
    E_a = float(sys.argv[1])
theta = theta/180.*3.141593
f_a = E_a*E_a + 2*E_a*m_a;
f_b = E_b*E_b + 2*E_b*m_b;
Ex = np.sqrt(np.power(E_b + m_b + m_Y - E_a - m_a, 2) - f_a - f_b + 
            2*np.cos(theta)*np.sqrt(f_a*f_b)) - m_X;
print('exitation energy of residual: %f (MeV), Ejectile kinematic energy: %f (MeV)' % (Ex, E_a))

