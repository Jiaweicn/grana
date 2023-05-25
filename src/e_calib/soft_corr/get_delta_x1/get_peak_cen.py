#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type

# get peaks centers by gaus fit

import ROOT as root
import numpy as np
import sys


def get_mean(h):
    nbins = h.GetNbinsX()
    max_val = 0
    for i in range(nbins):
        if max_val < h.GetBinContent(i):
            max_val = h.GetBinContent(i)
            mean_val = h.GetBinCenter(i)
    return mean_val


if len(sys.argv) != 5:
    print('usage: ./xxx <root file> <cut file> <min A> <max A>')
    sys.exit()

min_a = float(sys.argv[3])
max_a = float(sys.argv[4])
canv = root.TCanvas()
rootf = root.TFile(sys.argv[1])
t = rootf.Get('gr_tree_ang_softcorr')
rootf1 = root.TFile(sys.argv[2])
cut = rootf1.Get('CUTG')
h2 = root.TH1F('h2', 'h2', 1600, -350, 350)
t.Draw("x_corr>>h2",
    "CUTG && B_tar > %f && B_tar < %f"%(min_a, max_a))
mean = get_mean(h2)
h2.Fit('gaus','','',mean-1.7, mean+1.7)
mean = root.gROOT.GetFunction('gaus').GetParameter(1)
h2.Fit('gaus','','',mean-1.7, mean+1.7)
h2.GetXaxis().SetRangeUser(mean-5, mean+5)
canv.Update()
canv.SaveAs('%s_%s_A_%.1f_%.1f.png' % 
        (sys.argv[1].split('/')[-1], sys.argv[2].split('/')[-1], min_a, max_a))
print('peak center: %.2f' % root.gROOT.GetFunction('gaus').GetParameter(1))
