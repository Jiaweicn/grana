#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type

import ROOT  as root
import numpy as np



garb_lst = []

def get_xyz():
    with open('delta_x.txt') as f:
        lines = f.readlines()
    n = len(lines)//11
    x = []
    y = []
    z = []
    k = 0
    for i in range(n):
        for j in range(11):
            center = float(lines[i*11+5].split()[2])
            tmp = lines[k].split()
            x.append(float(tmp[2]))
            y.append(float(tmp[4]))
            z.append(float(tmp[2]) - center)
            k += 1
    x = np.array(x, float)
    y = np.array(y, float)
    z = np.array(z, float)
    return (x,y,z)


def draw_f1_gr(f1):
    with open('delta_x.txt') as f:
        lines = f.readlines()
    n = len(lines)//11
    k = 0
    for i in range(n):
        x = []
        y = []
        xx = []
        yy = []
        for j in range(11):
            center = float(lines[i*11+5].split()[2])
            tmp = lines[k].split()
            A1 = float(tmp[4])
            x1 = float(tmp[2])
            delta_x = f1.Eval(x1, A1)
            x.append(center + delta_x)
            y.append(float(A1))
            xx.append(x1)
            yy.append(A1)
            k += 1
        x = np.array(x, float)
        y = np.array(y, float)
        xx = np.array(xx, float)
        yy = np.array(yy, float)
        gr = root.TGraph(len(x), x, y)
        gr2 = root.TGraph(len(x), xx, yy)
        gr.Draw('lsame')
        gr2.SetMarkerStyle(20)
        gr2.SetMarkerSize(1.5)
        gr2.SetMarkerColor(root.kRed)
        gr2.Draw('samep')
        garb_lst.append(gr)
        garb_lst.append(gr2)




canv = root.TCanvas()
canv.Divide(1,2)
canv.cd(1)
x,y,z = get_xyz()
gr = root.TGraph2D(len(x), x, y, z)
gr.Draw('surf1')
f1 = root.TF2('f1', '[0] + [1]*x +[2]*x*x + [3]*x*x*x +[4]*x*x*x*x + [5]*y + [6]*x*y +[7]*x*x*y + [8]*x*x*x*y +[9]*x*x*x*x*y  +  [10]*y*y + [11]*x*y*y +[12]*x*x*y*y + [13]*x*x*x*y*y +[14]*x*x*x*x*y*y  +  [15]*y*y*y + [16]*x*y*y*y +[17]*x*x*y*y*y + [18]*x*x*x*y*y*y +[19]*x*x*x*x*y*y*y   + [20]*y*y*y*y + [21]*x*y*y*y*y +[22]*x*x*y*y*y*y + [23]*x*x*x*y*y*y*y +[24]*x*x*x*x*y*y*y*y', -350, 300, -1.5, 1)
gr.Fit(f1, 'R')
gr.Fit(f1, 'R')
canv.cd(2)
h2 = root.TH2F('h2', 'h2', 100, -350,300, 100, -2 ,2)
h2.Draw()
draw_f1_gr(f1)
canv.Update()

input('paused...')
