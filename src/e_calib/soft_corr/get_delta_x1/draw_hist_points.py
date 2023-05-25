#!/bin/python
# -*- coding: UTF-8 -*-
__metaclass__ = type

# draw the peak center points on top of A vs. X 2d hist


import ROOT as root
import numpy as np

garb_lst = []


def get_graph():
    with open('delta_x.txt') as f:
        lines = f.readlines()
    x = []
    y = []
    for line in lines:
        tmp = line.split()
        x.append(float(tmp[2]))
        y.append(float(tmp[4]))
    x = np.array(x, float)
    y = np.array(y, float)
    gr = root.TGraph(len(x), x, y)
    gr.SetMarkerSize(1.5)
    gr.SetMarkerStyle(20)
    gr.SetMarkerColor(root.kRed)
    garb_lst.append(gr)
    return gr


def get_hist(run):
    root_f = root.TFile('../soft_corr2/root_file_a_corr/run' + run + '_a_corr.root')
    t = root_f.Get('gr_tree_ang_softcorr')
    h = root.TH2F(run, run, 900, -350, 300, 900, -2.5, 2.5)
    t.Draw('B_tar:x_corr>>'+run, '', 'colz')
    garb_lst.append(root_f)
    garb_lst.append(h)
    garb_lst.append(t)
    return h
    

canv = root.TCanvas()
canv.Divide(1,3)

i = 1
gr = get_graph()
for run in ('2074', '2075_2078', '2079'):
    canv.cd(i)
    h = get_hist(run)
    #h.Draw('colz')
    gr.Draw('P')
    i += 1

canv.Update()
input('paused...')
