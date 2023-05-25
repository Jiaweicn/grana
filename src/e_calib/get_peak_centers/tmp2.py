#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type


import sys
import numpy as np
import ROOT  as root


f = root.TFile('root_file_ab_corr/a.root')
h1 = f.Get('h1')
n_bins = h1.GetNbinsX()
for i in range(n_bins):
    x = h1.GetBinCenter(i)
    y = h1.GetBinContent(i)
    print('%f  %f' % (x, y))
