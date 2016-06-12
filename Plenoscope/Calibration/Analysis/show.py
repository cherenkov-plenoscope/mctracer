#!/usr/bin/env python
from __future__ import absolute_import, print_function, division
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.patches import RegularPolygon
from matplotlib.patches import Circle
from matplotlib.collections import PatchCollection
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import gridspec
import mpl_toolkits.mplot3d.art3d as art3d
from mpl_toolkits.axes_grid1 import make_axes_locatable
import os

class LixelStatistics(object):
    def __init__(self, path2lixel_statistics_condensate):
        self.__path2lixel_statistics_condensate = os.path.abspath(path2lixel_statistics_condensate)
        self.__path2lixel_positions = os.path.join(os.path.dirname(self.__path2lixel_statistics_condensate), 'lixel_positions.csv')
       
        lixel_statistics = np.genfromtxt(self.__path2lixel_statistics_condensate)      
        self.geometric_efficiency = lixel_statistics[:,0]
        self.geometric_efficiency[np.isnan(self.geometric_efficiency)] = 0.0

        self.cx_mean = lixel_statistics[:,1]
        self.cx_std = lixel_statistics[:,2]

        self.cy_mean = lixel_statistics[:,3]
        self.cy_std = lixel_statistics[:,4]

        self.x_mean = lixel_statistics[:,5]
        self.x_std = lixel_statistics[:,6]

        self.y_mean = lixel_statistics[:,7]
        self.y_std = lixel_statistics[:,8]

        self.time_mean = lixel_statistics[:,9]
        self.time_std = lixel_statistics[:,10]

        self.__read_lixel_positions_and_outer_radius(self.__path2lixel_positions)
        self.__read_pixel_and_paxel_number(self.__path2lixel_statistics_condensate)

    def __read_pixel_and_paxel_number(self, path):
        self.number_pixel = 0
        self.number_paxel = 0
        self.photons_emitted_per_lixel = 0.0

        # pixel: 8431
        # paxel: 127
        # photons_emitted_per_lixel: 23.3484
        with open(path) as f:
            for i, line in enumerate(f):
                if "pixel" in line:
                    self.number_pixel = float(line[9:].strip('\n'))

                if "paxel" in line:
                    self.number_paxel = float(line[9:].strip('\n'))

                if "photons_emitted_per_lixel" in line:
                    self.photons_emitted_per_lixel = float(line[29:].strip('\n'))

                if i > 100:
                    break

        self.number_lixel = self.number_paxel*self.number_pixel

    def __read_lixel_positions_and_outer_radius(self, path):
        self.paxel_sensor_positions = np.genfromtxt(path)
        self.lixel_outer_radius = 0.0
        self.lixel_z_orientation = 0.0
        self.photons_emitted_per_lixel = 0.0

        with open(path) as f:
            for i, line in enumerate(f):
                if "lixel_outer_radius" in line:
                    self.lixel_outer_radius = float(line[23:].strip('\n'))

                if "lixel_z_orientation" in line:
                    self.lixel_z_orientation = float(line[24:].strip('\n'))

                if "photons_emitted_per_lixel" in line:
                    self.photons_emitted_per_lixel = float(line[29:].strip('\n'))

                if i > 100:
                    break

def add_to_ax_symmetric_hist(vals, ax, nbins=None):
    if nbins == None:
        nbins = np.sqrt(vals.shape[0]) 
    bins, bin_esdges = np.histogram(vals, bins=nbins)
    bin_centers = 0.5*(bin_esdges[1:]+bin_esdges[:-1])
    ax.set_xlim([1.025*bin_esdges[0], 1.025*bin_esdges[-1]])
    ax.step(bin_centers, bins)

def add_to_ax_cx_mean_hist(lss, ax):
    cx_mean = lss.cx_mean[lss.geometric_efficiency > 0.0]
    cx_mean = np.rad2deg(cx_mean)
    add_to_ax_symmetric_hist(cx_mean, ax)
    ax.set_xlabel('incoming direction mean c'+'x'+'/deg')

def add_to_ax_cy_mean_hist(lss, ax):
    cy_mean = lss.cy_mean[lss.geometric_efficiency > 0.0]
    cy_mean = np.rad2deg(cy_mean)
    add_to_ax_symmetric_hist(cy_mean, ax)
    ax.set_xlabel('incoming direction mean c'+'y'+'/deg')

def add_to_ax_x_mean_hist(lss, ax):
    x_mean = lss.x_mean[lss.geometric_efficiency > 0.0]
    add_to_ax_symmetric_hist(x_mean, ax)
    ax.set_xlabel('principal apertur mean '+'x'+'/m')

def add_to_ax_y_mean_hist(lss, ax):
    y_mean = lss.y_mean[lss.geometric_efficiency > 0.0]
    add_to_ax_symmetric_hist(y_mean, ax)
    ax.set_xlabel('principal apertur mean '+'y'+'/m')

def add_to_ax_cx_std_hist(lss, ax):
    cx_std = lss.cx_std[lss.geometric_efficiency > 0.0]
    cx_std = cx_std[~np.isnan(cx_std)]
    cx_std = np.rad2deg(cx_std)
    add_to_ax_symmetric_hist(cx_std, ax)
    ax.set_xlabel('incoming direction stddev c'+'x'+'/deg')

def add_to_ax_cy_std_hist(lss, ax):
    cy_std = lss.cy_std[lss.geometric_efficiency > 0.0]
    cy_std = cy_std[~np.isnan(cy_std)]
    cy_std = np.rad2deg(cy_std)
    add_to_ax_symmetric_hist(cy_std, ax)
    ax.set_xlabel('incoming direction stddev c'+'y'+'/deg')

def add_to_ax_time_mean_hist(lss, ax):
    time_mean = lss.time_mean[lss.geometric_efficiency > 0.0]
    add_to_ax_symmetric_hist(time_mean, ax)
    ax.set_xlabel('relative arrival time mean t/s')

def add_to_ax_time_std_hist(lss, ax):
    time_std = lss.time_std[lss.geometric_efficiency > 0.0]
    time_std = time_std[~np.isnan(time_std)]
    add_to_ax_symmetric_hist(time_std, ax)
    ax.set_xlabel('relative arrival time stddev t/s')

def add_to_ax_geometric_efficieny_hist(lss, ax):
    geo_eff = lss.geometric_efficiency
    add_to_ax_symmetric_hist(geo_eff, ax, nbins=np.floor(lss.photons_emitted_per_lixel))
    ax.set_xlabel('geometric efficiency eff/1')