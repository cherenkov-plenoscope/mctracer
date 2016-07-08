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
from tqdm import tqdm
from mpl_toolkits.axes_grid1 import make_axes_locatable

import subprocess
import os
from glob import glob

from lightfield import PlenoscopeLightFieldCalibration
from lightfield import LightField
import tempfile
import pickle
from copy import deepcopy

def add_to_ax(ax, I, px, py, colormap='viridis', hexrotation=30, vmin=None, vmax=None):

    if vmin is None:
        vmin = I.min()
    if vmax is None:
        vmax = I.max()
    fov = np.abs(px).max() * 1.05
    Area = fov * fov
    bin_radius = 1.15 * np.sqrt((Area/I.shape[0]))

    nfov = fov + bin_radius
    ax.set_xlim([-nfov, nfov])
    ax.set_ylim([-nfov, nfov])
    ax.set_aspect("equal")
    
    orientation=np.deg2rad(hexrotation)

    patches = []
    for d in range(I.shape[0]):
        patches.append(
            RegularPolygon(
                (px[d], py[d]),
                numVertices=6,
                radius=bin_radius,
                orientation=orientation
            )
        )

    p = PatchCollection(patches, cmap=colormap, alpha=1, edgecolor='none')

    p.set_array(I)
    p.set_clim([vmin, vmax])

    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    plt.colorbar(p, cax=cax)

    ax.add_collection(p)
    ax.set_aspect('equal')

    return p 

def plot_pixel(img, ax, colormap="viridis", vmin=None, vmax=None):
    ax.set_xlabel('x/deg')
    ax.set_ylabel('y/deg')
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    add_to_ax(
        ax=ax,
        I=img.intensity,
        px=np.rad2deg(img.pixel_pos.x),
        py=np.rad2deg(img.pixel_pos.y),
        colormap=colormap,
        hexrotation=30,
        vmin=vmin,
        vmax=vmax,
    )

def plot_paxel(img, ax, colormap="viridis", vmin=None, vmax=None):
    ax.set_xlabel('x/m')
    ax.set_ylabel('y/m')
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    add_to_ax(
        ax=ax,
        I=img.intensity,
        px=img.pixel_pos.x,
        py=img.pixel_pos.y,
        colormap=colormap,
        hexrotation=0,
        vmin=vmin,
        vmax=vmax,
    )

class FigureSize():

    def __init__(self, cols, rows, dpi):
        self.cols = cols
        self.rows = rows
        self.dpi = dpi
        self.hight = self.rows/self.dpi
        self.width = self.cols/self.dpi

def get_n_highest(lf, n):
    I = lf.I
    flat_idxs = np.argsort(I.flatten())[-n:]
    flat_mask = np.zeros(shape=I.shape[0]*I.shape[1], dtype=bool)
    flat_mask[flat_idxs] = True
    return flat_mask.reshape(I.shape)

def save_principal_aperture_arrival_stack(lf, steps=6, n_channels=137, outprefix='aperture3D'):

    plt.rcParams.update({'font.size': 12})
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')

    fsz = FigureSize(1920, 1080, dpi=240)
    fig = plt.figure(figsize=(fsz.width, fsz.hight))
    ax = fig.gca(projection='3d')

    above_threshold = get_n_highest(lf, n_channels)

    min_t = lf.t[above_threshold].min()
    dur_t = lf.t[above_threshold].max() - min_t
    max_I = lf.I[above_threshold].max()

    for pax in range(lf.n_paxel):
        for pix in range(lf.n_pixel):
            if above_threshold[pix, pax]:
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
                ax.scatter(x[1], y[1], z[1], lw=0, s=35., alpha=I**2)

    p = Circle((0, 0), 25, edgecolor='k', facecolor='none', lw=1.)
    ax.add_patch(p)
    art3d.pathpatch_2d_to_3d(p, z=0, zdir="z")

    ax.set_xlim(-25, 25)
    ax.set_ylim(-25, 25)
    ax.set_zlim(0, dur_t)
    ax.set_xlabel('X/m')
    ax.set_ylabel('Y/m')
    ax.set_zlabel('t/s')

    ndigits = int(np.ceil(np.log10(steps)))
    azimuths = np.linspace(0., 360., steps, endpoint=False)
    for i, azimuth in enumerate(azimuths):
        ax.view_init(elev=5., azim=azimuth)
        plt.subplots_adjust(left=0.01, right=0.99, top=0.99, bottom=0.01)
        plt.savefig(outprefix+'_'+str(i).zfill(ndigits)+".png", dpi=fsz.dpi)

    plt.close()

def plot_sum_event(lf, thresh=0):
    plt.ion()

    # avoid side effects
    lf = deepcopy(lf)
    lf.I[lf.I < thresh] = 0
    
    fig, (ax_dir, ax_pap) = plt.subplots(1,2)
    plt.suptitle("Energy: "+lf.header["Primary particle"]["E"]+"\n"
        +"impact parameter:"+str(lf.header["core positions: "])
        )
    plot_pixel(lf.pixel_sum(), ax_dir)
    plot_paxel(lf.paxel_sum(), ax_pap)
    
def save_sum_projections(lf, outpath=None, thresh=0):
    if outpath is None:
        outpath = lf.path.path_wo_ext+'_sum_projection'

    # avoid side effects
    lf = deepcopy(lf)    
    lf.I[lf.I < thresh] = 0

    plt.rcParams.update({'font.size': 20})
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    fig, (ax_dir, ax_pap) = plt.subplots(1, 2, figsize=(16,9))
    
    plot_pixel(lf.pixel_sum(), ax_dir)

    lf.I[:, lf.paxel_eff<=0.4] = 0
    plot_paxel(lf.paxel_sum(), ax_pap)

    plt.subplots_adjust(left=0.05, right=0.95, top=0.95, bottom=0.05)
    plt.savefig(outpath+".png", dpi=120)
    plt.close()

def save_refocus_stack(lf, obj_dist_min, obj_dist_max, steps, outprefix='refocus', use_absolute_scale=True):

    plt.rcParams.update({'font.size': 12})
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')

    object_distances = np.logspace(
        np.log10(obj_dist_min),
        np.log10(obj_dist_max),
        steps
    )

    images = [lf.refocus(object_distance) for object_distance in object_distances]
    intensities = [i.intensity for i in images]
    if use_absolute_scale:
        vmin=np.array(intensities).min()
        vmax=np.array(intensities).max()
    else:
        vmin, vmax = None, None

    for i in range(len(images)):
        image = images[i]
        object_distance = object_distances[i]

        fig = plt.figure(figsize=(7, 6)) 
        fig, (ax_dir, ax_pap) = plt.subplots(1,2)
        plt.suptitle("Energy: "+lf.header["Primary particle"]["E"]+"\n"
            +"impact parameter:"+str(lf.header["core positions: "])
            )
        gs = gridspec.GridSpec(1, 2, width_ratios=[1, 6]) 
        ax0 = plt.subplot(gs[0])
        ax0.set_xlim([0, 1])
        ax0.set_ylim([0, obj_dist_max/1e3])
        ax0.yaxis.tick_left()
        ax0.set_ylabel('object distance/km')
        ax0.spines['right'].set_visible(False)
        ax0.spines['top'].set_visible(False)
        ax0.spines['bottom'].set_visible(False)
        ax0.xaxis.set_visible(False)
        ax0.plot([0, .5], [object_distance/1e3, object_distance/1e3], linewidth=5.0)
        ax0.text(0.0, -1.0, format(object_distance/1e3, '.2f')+r'\,km')

        ax1 = plt.subplot(gs[1])
        ax1.set_aspect('equal')  

        plot_pixel(image, ax1, vmin=vmin, vmax=vmax)

        ndigits = int(np.ceil(np.log10(steps)))
        plt.savefig(outprefix+'_'+str(i).zfill(ndigits)+".png", dpi=180)

        plt.close()

def save_refocus_gif(lf, steps=10, use_absolute_scale=True):
    with tempfile.TemporaryDirectory() as work_dir:
        save_refocus_stack(
            lf,
            0.75e3,
            15e3,
            steps,
            outprefix=work_dir+'/'+'refocus',
            use_absolute_scale=use_absolute_scale)

        subprocess.call(
            ['convert',
            work_dir+'/'+'refocus_*.png',
            '-set' ,'delay', '10',
            '-reverse',
            work_dir+'/'+'refocus_*.png',
            '-set' ,'delay', '10',
            '-loop', '0',
            lf.path.path_wo_ext+'_refocus.gif'
            ])

def save_aperture_photons_gif(lf, steps=72):

    T=20 #seconds
    spf = T/steps
    s100pf = int(spf*100.0)

    with tempfile.TemporaryDirectory() as work_dir:
        save_principal_aperture_arrival_stack(
            lf,
            steps=steps,
            n_channels=512,
            outprefix=work_dir+'/'+'aperture3D')
        subprocess.call(
            ['convert',
            work_dir+'/'+'aperture3D_*.png',
            '-set' ,'delay', str(s100pf),
            '-loop', '0',
            lf.path.path_wo_ext+'_aperture3D.gif'
            ])

def main():
    #plfc = PlenoscopeLightFieldCalibration('sub_pixel_statistics.txt')
    plfc = pickle.load(open("calibration", "rb"))    
    lf = LightField('1.txt', plfc)
    save_refocus_gif(lf)
    return lf

if __name__ == "__main__":
    lf = main()
    plot_sum_event(lf)