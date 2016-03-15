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
import mpl_toolkits.mplot3d.art3d as art3d
import scipy
import scipy.spatial
from tqdm import tqdm

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
                self.n_pixel = int(line[28:])

            if line[0:37] == '# number_of_principal_aperture_bins: ':
                self.n_paxel = int(line[37:])

        self.__post_init()

    def __post_init(self):
        self.n_cells = self.n_paxel*self.n_pixel
        self.paxel_pos = self.__estimate_principal_aperture_bin_positions()
        self.pixel_pos = self.__estimate_fov_direction_bin_positions()

    def __estimate_principal_aperture_bin_positions(self):
        paxel_pos = np.zeros(shape=(2, self.n_paxel))
        for PA_bin in range(self.n_paxel):
            paxel_pos[0, PA_bin] += np.nanmean(self.raw['x_mean'][PA_bin::self.n_paxel])
            paxel_pos[1, PA_bin] += np.nanmean(self.raw['y_mean'][PA_bin::self.n_paxel])

        return np.rec.fromarrays((paxel_pos[0,:], paxel_pos[1,:]), 
            dtype=[('x', 'f8'),('y', 'f8')]
        )

    def __estimate_fov_direction_bin_positions(self):
        pixel_pos = np.zeros(shape=(2, self.n_pixel))
        for dir_bin in range(self.n_pixel):
            pixel_pos[0, dir_bin] = np.nanmean(self.raw['cx_mean'][self.n_paxel*dir_bin:self.n_paxel*(dir_bin+1)])
            pixel_pos[1, dir_bin] = np.nanmean(self.raw['cy_mean'][self.n_paxel*dir_bin:self.n_paxel*(dir_bin+1)])

        return np.rec.fromarrays((pixel_pos[0,:], pixel_pos[1,:]), 
            dtype=[('x', 'f8'),('y', 'f8')]
        )

    def bin2pap_bin(self, abin):
        return abin%n_paxel

    def bin2dir_bin(self, abin):
        return int(abin/n_paxel)

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

        self.n_cells = self.raw.shape[0]

class LightField():
    
    def __init__(self, plfc, plf):

        self.__plc = plfc

        # INTENSITY
        where_sensitive = plfc.raw['geometrical_efficiency'] != 0.

        intensity = np.zeros(shape=plfc.n_cells)
        intensity[where_sensitive] = plf.raw['number_photons_raw'][where_sensitive]/plfc.raw['geometrical_efficiency'][where_sensitive]
        intensity[np.invert(where_sensitive)] = 0

        mean_sensitivity = plfc.raw['geometrical_efficiency'].sum()/plfc.n_cells
        intensity *= mean_sensitivity

        #ARRIVAL TIMES
        arrival_times = np.zeros(shape=plf.n_cells)
        arrival_times[where_sensitive] = plf.raw['sub_pixel_arrival_time'][where_sensitive] - plfc.raw['time_delay_from_principal_apertur_to_sub_pixel'][where_sensitive]
        arrival_times -= arrival_times.min()
        
        #reshape
        self.I = np.reshape(plf.raw['number_photons_raw'], newshape=(plfc.n_pixel, plfc.n_paxel))
        #self.I = np.reshape(intensity, newshape=(plfc.n_pixel, plfc.n_paxel))
        self.t = np.reshape(arrival_times, newshape=(plfc.n_pixel, plfc.n_paxel))

        self.n_cells = plfc.n_cells
        self.n_paxel = plfc.n_paxel
        self.n_pixel = plfc.n_pixel

        self.pixel_pos = plfc.pixel_pos
        self.paxel_pos = plfc.paxel_pos

        self.pixel_pos_tree = scipy.spatial.cKDTree(np.array([self.pixel_pos["x"], self.pixel_pos["y"]]).T)
        self.paxel_pos_tree = scipy.spatial.cKDTree(np.array([self.paxel_pos["x"], self.paxel_pos["y"]]).T)


def plot_3D(lf, thr=25):

    fig = plt.figure(figsize=(16, 9), dpi=120)
    ax = fig.gca(projection='3d')

    min_t = lf.t[lf.I>thr].min()
    dur_t = lf.t[lf.I>thr].max() - min_t
    max_I = lf.I[lf.I>thr].max()
    print('t min', min_t, 'dur', dur_t)

    for pax in range(lf.n_paxel):
        for pix in range(lf.n_pixel):
            if lf.I[pix, pax] > thr:

                d = (lf.t[pix, pax]-min_t)

                xpix = lf.pixel_pos['x'][pix]
                ypix = lf.pixel_pos['y'][pix]

                incident = np.array([
                    xpix,
                    ypix,
                    np.sqrt(1. -xpix*xpix - ypix*ypix)
                ])

                x = np.array([
                    lf.paxel_pos['x'][pax], 
                    lf.paxel_pos['x'][pax] + d*incident[0]
                ])
                y = np.array([
                    lf.paxel_pos['y'][pax], 
                    lf.paxel_pos['y'][pax] + d*incident[1]
                ])
                z = np.array([
                    0., 
                    d*incident[2]
                ])
                #ax.plot(x, y, z, 'b')
                I = lf.I[pix, pax]/max_I
                ax.scatter(x[1], y[1], z[1], lw=0, s=75., alpha=I**2)

    p = Circle((0, 0), 25, edgecolor='k', facecolor='none', lw=1.)
    ax.add_patch(p)
    art3d.pathpatch_2d_to_3d(p, z=0, zdir="z")

    ax.set_xlim(-25, 25)
    ax.set_ylim(-25, 25)
    ax.set_zlim(0, dur_t)
    ax.set_xlabel('X/m')
    ax.set_ylabel('Y/m')
    ax.set_zlabel('t/s')

    for ii in xrange(0,360,2):
        ax.view_init(elev=5., azim=ii)
        plt.savefig(str(ii).zfill(3)+".png")

def plot_sum_event(path, thresh=0):
    plt.ion()
    plf.load_epoch_160310(path)
    lf = LightField(plfc, plf)

    I = lf.I
    I[I<thresh]=0
    fig, (ax_dir, ax_pap) = plt.subplots(1,2)
    add_to_ax(ax_dir, np.sum(I, axis=1), lf.pixel_pos['x'], lf.pixel_pos['y'])
    add_to_ax(ax_pap, np.sum(I, axis=0), lf.paxel_pos['x'], lf.paxel_pos['y'])

def refocus(lf, wanted_object_distance):
    alpha = get_alpha(wanted_object_distance)
    refocused_image = np.zeros(lf.n_pixel)
    focal_length = 75.0

    dx = np.tan(lf.pixel_pos['x'])*focal_length
    dy = np.tan(lf.pixel_pos['y'])*focal_length

    px = lf.paxel_pos['x']
    py = lf.paxel_pos['y']

    dx_tick = px[:, np.newaxis] + (dx[np.newaxis, :] - px[:, np.newaxis])/alpha  # (127, 8400)
    dy_tick = py[:, np.newaxis] + (dy[np.newaxis, :] - py[:, np.newaxis])/alpha

    dx_des = np.arctan(dx_tick/focal_length)
    dy_des = np.arctan(dy_tick/focal_length)

    # (127*8400, 2)
    desired_x_and_y = np.zeros((np.prod(dx_des.shape), 2))
    desired_x_and_y[:, 0] = dx_des.flatten()
    desired_x_and_y[:, 1] = dy_des.flatten()

    nearest_pixel_distances, nearest_pixel_indices = lf.pixel_pos_tree.query(desired_x_and_y)

    summanden = lf.I[nearest_pixel_indices, np.arange(lf.n_paxel).repeat(lf.n_pixel)]
    summanden = summanden.reshape(lf.n_paxel, lf.n_pixel)
    refocused_image = summanden.sum(axis=0)

    return refocused_image

def get_alpha(wanted_object_distance):
    focal_length = 75.0
    initial_object_distance = 10e3
    initial_image_distance = 1/(1/focal_length - 1/initial_object_distance)
    alpha = 1/initial_image_distance * 1/((1/focal_length) - (1/wanted_object_distance))
    return alpha


plfc = PlenoscopeLightFieldCalibration()
plfc.load_plenoscope_calibration_epoch_160310('/home/dneise/mct/light_field_calibration/sub_pixel_statistics.txt')

plf = PlenoscopeLightField()
plf.load_epoch_160310('/home/dneise/mct/He/1.txt')

lf = LightField(plfc, plf)