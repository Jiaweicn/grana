#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type


import ROOT as root
import numpy as np
#import sys


def get_gr(lines, brho):
    x = []
    y = []
    for line in lines:
        tmp = line.split()
        if tmp[1] == '8.438':
            continue
        x.append(float(tmp[3]))
        y.append(float(tmp[5]))
    if brho == 100:
        x0 = 58
        scale_f = 1
    elif brho == 101:
        x0 = 56.28
        scale_f = 1.010
    elif brho == 102:
        x0 = 52.67
        scale_f = 1.021

    x = np.array(x, float) - x0
    y = np.array(y, float)
    return root.TGraph(len(x), x, y/scale_f)


with open('peak_centers.txt') as f:
    lines = f.readlines()

gr1 = get_gr(lines[2:9], 100)
gr2 = get_gr(lines[9:18], 101)
gr3 = get_gr(lines[18:26], 102)

gr1.SetMarkerStyle(8)
gr1.SetMarkerSize(1)
gr1.SetMarkerColor(root.kBlack)
gr1.SetLineColor(root.kBlack)
gr2.SetMarkerStyle(8)
gr2.SetMarkerSize(1)
gr2.SetMarkerColor(root.kBlue)
gr2.SetLineColor(root.kBlue)
gr3.SetMarkerStyle(8)
gr3.SetMarkerSize(1)
gr3.SetMarkerColor(root.kRed)
gr3.SetLineColor(root.kRed)

h = root.TH2F('h', 'h', 100, -400, 200, 100, -15, 35)
h.Draw()
gr1.Draw('samelp')
gr2.Draw('samelp')
gr3.Draw('samelp')

leg = root.TLegend(.6,.6,.9,.9)
leg.AddEntry(gr1, 'Brho=100', 'lp')
leg.AddEntry(gr2, 'Brho=101', 'lp')
leg.AddEntry(gr3, 'Brho=102', 'lp')
leg.Draw()

# get offsets
#f1 = root.TF1('f1', '[0]*x*x + [1]*x +[2]', -400, -50)
#canv2 = root.TCanvas()
#gr3.Draw('ap')
#gr3.Fit(f1)
#gr3.Fit(f1)
#gr2.Draw('psame')
#x = gr2.GetX()
#y = gr2.GetY()
#chi2 = 0
#for i in range(len(x)-2):
    #chi2 += (y[2+i] - f1.Eval(x[2+i])) * (y[2+i] - f1.Eval(x[2+i]))
#print('chi2 = ' + str(chi2))

# get uncertainty
#f1 = root.TF1('f1', '[0]*x*x + [1]*x +[2]', -400, -50)
#canv2 = root.TCanvas()
#gr2.Draw('ap')
#gr2.Fit(f1)
#gr2.Fit(f1)
#gr3.Draw('psame')
#x = gr3.GetX()
#p0 = 1237.36
#a = 1.021
#for the_x in x:
#    delta_x = the_x
#    print('p = %f' % (p0+a*f1.Eval(delta_x)))

input('paused...')
