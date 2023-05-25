#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type


import ROOT as root
import numpy as np

f = root.TFile('run2074_ab_corr.root')
t = f.Get('gr_tree_ang_softcorr_all')
h1 = root.TH1F('h1', 'h1', 4000, -350, 350)
h2 = root.TH1F('h2', 'h2', 4000, -350, 350)
h3 = root.TH1F('h3', 'h3', 4000, -350, 350)
h4 = root.TH1F('h4', 'h4', 4000, -350, 350)
canv = root.TCanvas()
canv.Divide(1,4)
canv.cd(1)
t.Draw("x_corr>>h1", "A_tar>-1.5&&A_tar<0.7&&B_tar>-2.5&&B_tar<2&&sqrt((A_tar+4.5)*(A_tar+4.5)+(B_tar*B_tar))<5.0")
canv.cd(2)
t.Draw("x_corr>>h2", "A_tar>-1.5&&A_tar<0.7&&B_tar>-2.5&&B_tar<2&&sqrt((A_tar+4.5)*(A_tar+4.5)+(B_tar*B_tar))<4.5")
canv.cd(3)
t.Draw("x_corr>>h3", "A_tar>-1.5&&A_tar<0.7&&B_tar>-2.5&&B_tar<2&&sqrt((A_tar+4.5)*(A_tar+4.5)+(B_tar*B_tar))<4.0")
canv.cd(4)
t.Draw("x_corr>>h4", "A_tar>-1.5&&A_tar<0.7&&B_tar>-2.5&&B_tar<2&&sqrt((A_tar+4.5)*(A_tar+4.5)+(B_tar*B_tar))<3.5")

f1 = root.TF1('f1', 'gaus(0)+gaus(3)+[6]', -143, -137)
f1.SetParameter(0, 1000)
f1.SetParameter(1, -141.5)
f1.SetParameter(2, 0.8)
f1.SetParameter(3, 500)
f1.SetParameter(4, -138.5)
f1.SetParameter(5, 0.8)
f1.SetParameter(6, 0)

f2 = root.TF1('f2', 'gaus(0)+gaus(3)+[6]', -143, -137)
f2.SetParameter(0, 1000)
f2.SetParameter(1, -141.5)
f2.SetParameter(2, 0.8)
f2.SetParameter(3, 500)
f2.SetParameter(4, -138.5)
f2.SetParameter(5, 0.8)
f2.SetParameter(6, 0)

f3 = root.TF1('f3', 'gaus(0)+gaus(3)+[6]', -143, -137)
f3.SetParameter(0, 1000)
f3.SetParameter(1, -141.5)
f3.SetParameter(2, 0.8)
f3.SetParameter(3, 500)
f3.SetParameter(4, -138.5)
f3.SetParameter(5, 0.8)
f3.SetParameter(6, 0)

f4 = root.TF1('f4', 'gaus(0)+gaus(3)+[6]', -143, -137)
f4.SetParameter(0, 1000)
f4.SetParameter(1, -141.5)
f4.SetParameter(2, 0.8)
f4.SetParameter(3, 500)
f4.SetParameter(4, -138.5)
f4.SetParameter(5, 0.8)
f4.SetParameter(6, 0)

print('fitting h1...')
h1.Fit(f1, 'r')
h1.Fit(f1, 'r')
h1.Fit(f1, 'r')

print('fitting h2...')
h2.Fit(f2, 'r')
h2.Fit(f2, 'r')
h2.Fit(f2, 'r')

print('fitting h3...')
h3.Fit(f3, 'r')
h3.Fit(f3, 'r')
h3.Fit(f3, 'r')

print('fitting h4...')
h4.Fit(f4, 'r')
h4.Fit(f4, 'r')
h4.Fit(f4, 'r')

input('paused....')
