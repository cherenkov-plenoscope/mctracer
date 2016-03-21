#!/usr/bin/env python
from __future__ import absolute_import, print_function, division
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

path = '/home/sebastian/Desktop/mctout/perfect_events/He/1_0'

def load_perfect_plenoscope_event(path):
    raw = np.genfromtxt(path)
    # 0 1   2       3       4       5   6   7
    # x y   cos_x   cos_y   lambda  t   id  h
    evt = np.rec.fromarrays(
        (
            raw[:,0], 
            raw[:,1], 
            5000.0*np.ones(shape=raw[:,1].shape), 
            raw[:,2], 
            raw[:,3], 
            raw[:,4], 
            raw[:,5], 
            raw[:,7]
        ), 
        dtype=[
            ('x', 'f8'),
            ('y', 'f8'),
            ('z', 'f8'),
            ('cx', 'f8'),
            ('cy', 'f8'),
            ('wavelength', 'f8'),
            ('t', 'f8'),
            ('prod_z', 'f8'),
        ]
    )

    return evt

def production_positions(evt):

    incident = np.array([
        evt['cx'], 
        evt['cy'], 
        np.sqrt(1. -evt['cx']*evt['cx'] - evt['cy']*evt['cy'])
    ])

    a = (evt['prod_z'])/incident[2,:]

    print(a[0:5]*incident[0,0:5])
    prod_x = evt['x'] - a*incident[0,:]
    prod_y = evt['y'] - a*incident[1,:]
    print(prod_x[0:5])

    return np.array([prod_x, prod_y, evt['prod_z']]), incident

def plot_few_production_positions(prod, incident, mask):

    a = 100.

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    for i in range(prod[0,:].shape[0]):
        if mask[i] and i%10==0:
            x = np.array([
                prod[0,i],
                prod[0,i] - a*incident[0,i]
            ])

            y = np.array([
                prod[1,i],
                prod[1,i] - a*incident[1,i]
            ])

            z = np.array([
                prod[2,i],
                prod[2,i] - a*incident[2,i]
            ])

            #ax.plot(x, y, z, 'b')
            ax.scatter(prod[0,i], prod[1,i], prod[2,i], lw=0, alpha=0.1)
    plt.show()

evt = load_perfect_plenoscope_event(path)

[p,inc]  = production_positions(evt)
