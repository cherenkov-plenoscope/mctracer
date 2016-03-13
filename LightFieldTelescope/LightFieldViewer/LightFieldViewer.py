#!/usr/bin/env python
from __future__ import absolute_import, print_function, division
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.patches import RegularPolygon
from matplotlib.collections import PatchCollection
from mpl_toolkits.mplot3d import Axes3D

def add_to_ax(ax, I, px, py):

    fov = np.abs(px).max()*1.05 
    Area = fov*fov
    bin_radius = np.sqrt((Area/I.shape[0]))

    nfov = fov+bin_radius
    ax.set_xlim([-nfov, nfov])
    ax.set_ylim([-nfov, nfov])
    I/=I.max()

    for d in range(I.shape[0]):
        circle1=plt.Circle( (px[d], py[d]), bin_radius, color=str(I[d]) )
        ax.add_artist(circle1)
    ax.set_aspect('equal')        

class PlenoscopeLightFieldCalibration():

    def load_plenoscope_calibration_epoch_160310(self, plenoscope_calib_path):
        
        raw = np.genfromtxt(plenoscope_calib_path)
        
        # geometrical_efficiency[1] cx[rad] cy[rad] x[m]    y[m]    t[s]
        self.raw = np.rec.fromarrays(
            (raw[:,0], raw[:,1], raw[:,2], raw[:,3], raw[:,4], raw[:,5]), 
            dtype=[
                ('geometrical_efficiency', 'f8'),
                ('cx_mean', 'f8'),
                ('cy_mean', 'f8'),
                ('x_mean', 'f8'),
                ('y_mean', 'f8'),
                ('time_delay_from_principal_apertur_to_sub_pixel', 'f8'),
            ]
        )

        f = open(plenoscope_calib_path)
        for i in range(100):
            line = f.readline()
            if line[0:28] == '# number_of_direction_bins: ':
                self.n_dir_bins = int(line[28:])

            if line[0:37] == '# number_of_principal_aperture_bins: ':
                self.n_pap_bins = int(line[37:])

        self.__post_init()

    def __post_init(self):
        self.n_bins = self.n_pap_bins*self.n_dir_bins
        self.pap_bin_positions = self.__estimate_principal_aperture_bin_positions()
        self.dir_bin_positions = self.__estimate_fov_direction_bin_positions()

    def __estimate_principal_aperture_bin_positions(self):
        pap_bin_positions = np.zeros(shape=(2, self.n_pap_bins))
        for PA_bin in range(self.n_pap_bins):
            pap_bin_positions[0, PA_bin] += np.nanmean(self.raw['x_mean'][PA_bin::self.n_pap_bins])
            pap_bin_positions[1, PA_bin] += np.nanmean(self.raw['y_mean'][PA_bin::self.n_pap_bins])

        return np.rec.fromarrays((pap_bin_positions[0,:], pap_bin_positions[1,:]), 
            dtype=[('x', 'f8'),('y', 'f8')]
        )

    def __estimate_fov_direction_bin_positions(self):
        dir_bin_positions = np.zeros(shape=(2, self.n_dir_bins))
        for dir_bin in range(self.n_dir_bins):
            dir_bin_positions[0, dir_bin] = np.nanmean(self.raw['cx_mean'][self.n_pap_bins*dir_bin:self.n_pap_bins*(dir_bin+1)])
            dir_bin_positions[1, dir_bin] = np.nanmean(self.raw['cy_mean'][self.n_pap_bins*dir_bin:self.n_pap_bins*(dir_bin+1)])

        return np.rec.fromarrays((dir_bin_positions[0,:], dir_bin_positions[1,:]), 
            dtype=[('cx', 'f8'),('cy', 'f8')]
        )

    def bin2pap_bin(self, abin):
        return abin%n_pap_bins

    def bin2dir_bin(self, abin):
        return int(abin/n_pap_bins)

class PlenoscopeLightField():

    def load_epoch_160310(self, path):
        raw = np.genfromtxt(path)
        self.raw = np.rec.fromarrays(
            (raw[:,0], raw[:,1]), 
            dtype=[
                ('sub_pixel_arrival_time', 'f8'),
                ('number_photons_raw', 'f8')
            ]
        )

        self.n_bins = self.raw.shape[0]

class LightField():
    
    def __init__(self, plfc, plf):

        self.__plc = plfc

        # INTENSITY
        where_sensitive = plfc.raw['geometrical_efficiency'] != 0.

        intensity = np.zeros(shape=plfc.n_bins)
        intensity[where_sensitive] = plf.raw['number_photons_raw'][where_sensitive]/plfc.raw['geometrical_efficiency'][where_sensitive]
        intensity[np.invert(where_sensitive)] = 0

        mean_sensitivity = plfc.raw['geometrical_efficiency'].sum()/plfc.n_bins
        intensity *= mean_sensitivity

        #ARRIVAL TIMES
        arrival_times = np.zeros(shape=plf.n_bins)
        arrival_times[where_sensitive] = plf.raw['sub_pixel_arrival_time'][where_sensitive] - plfc.raw['time_delay_from_principal_apertur_to_sub_pixel'][where_sensitive]
        arrival_times -= arrival_times.min()
        
        #reshape
        self.I = np.reshape(plf.raw['number_photons_raw'], newshape=(plfc.n_dir_bins, plfc.n_pap_bins))
        #self.I = np.reshape(intensity, newshape=(plfc.n_dir_bins, plfc.n_pap_bins))
        self.t = np.reshape(arrival_times, newshape=(plfc.n_dir_bins, plfc.n_pap_bins))

        self.n_bins = plfc.n_bins
        self.n_pap_bins = plfc.n_pap_bins
        self.n_dir_bins = plfc.n_dir_bins

        self.dir_bin_positions = plfc.dir_bin_positions
        self.pap_bin_positions = plfc.pap_bin_positions

plfc = PlenoscopeLightFieldCalibration()
plfc.load_plenoscope_calibration_epoch_160310('../light_field_calibration/sub_pixel_statistics.txt')

plf = PlenoscopeLightField()
#plf.load_epoch_160310('He/1.txt')

#lf = LightField(plfc, plf)


from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

def plot_3D(lf, thr=25):

    fig = plt.figure()
    ax = fig.gca(projection='3d')

    min_t = lf.t[lf.I>thr].min()
    max_I = lf.I[lf.I>thr].max()

    for pap_bin in range(lf.n_pap_bins):
        for dir_bin in range(lf.n_dir_bins):
            if lf.I[dir_bin, pap_bin] > thr:

                d = (lf.t[dir_bin, pap_bin]-min_t)*3e8

                x_off = lf.dir_bin_positions['cx'][dir_bin]
                y_off = lf.dir_bin_positions['cy'][dir_bin]

                incident = np.array([
                    x_off,
                    y_off,
                    np.sqrt(1. -x_off*x_off - y_off*y_off)
                ])

                x = np.array([
                    lf.pap_bin_positions['x'][pap_bin], 
                    lf.pap_bin_positions['x'][pap_bin] + d*incident[0]
                ])
                y = np.array([
                    lf.pap_bin_positions['y'][pap_bin], 
                    lf.pap_bin_positions['y'][pap_bin] + d*incident[1]
                ])
                z = np.array([
                    0., 
                    d*incident[2]
                ])
                #ax.plot(x, y, z, 'b')
                I = lf.I[dir_bin, pap_bin]/max_I*5.0
                ax.scatter(x[1], y[1], z[1], s=I**3.0)

    ax.set_xlim(-25, 25)
    ax.set_ylim(-25, 25)
    ax.set_zlim(0, 5)

def plot_sum_event(path, thresh=0):
    plt.ion()
    plf.load_epoch_160310(path)
    lf = LightField(plfc, plf)

    I = lf.I
    I[I<thresh]=0
    fig, (ax_dir, ax_pap) = plt.subplots(1,2)
    add_to_ax(ax_dir, np.sum(lf.I, axis=1), lf.dir_bin_positions['cx'], lf.dir_bin_positions['cy'])
    add_to_ax(ax_pap, np.sum(lf.I, axis=0), lf.pap_bin_positions['x'],  lf.pap_bin_positions['y'])

    plot_3D(lf, thresh)

