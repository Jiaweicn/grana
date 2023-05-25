#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type


import ROOT as root
import numpy as np


def get_gr():
    with open('peak_centers.txt') as f:
        lines = f.readlines()
    x = []
    y = []
    for line in lines[2:]:
        tmp = line.split()
        if tmp[1] == '8.438':
            continue
        B = tmp[0]
        if B == '100':
            scale_f = 1
            x0 = 58
        elif B == '101':
            scale_f = 1.010
            x0 = 56.28
        elif B == '102':
            scale_f = 1.021
            x0 = 52.67
        x.append(float(tmp[3])-x0)
        y.append(float(tmp[5])/scale_f)
    x,y = zip(*sorted(zip(x,y)))
    x = np.array(x, float)
    y = np.array(y, float)
    return root.TGraph(len(x), x, y)


gr = get_gr()
h = root.TH2F('h', 'h', 100, -450, 400, 100, -15, 35)
h.Draw()
gr.Draw('same*')
f1 = root.TF1('f1', '[0]*x*x + [1]*x +[2]', -450, 400)
gr.Fit(f1)
gr.Fit(f1)

input('paused...')


