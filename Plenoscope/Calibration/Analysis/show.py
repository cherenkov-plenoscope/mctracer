 #!/usr/bin/env python
from __future__ import absolute_import, print_function, division
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.patches import RegularPolygon
from matplotlib.collections import RegularPolyCollection, PolyCollection
from matplotlib.patches import Circle
from matplotlib.collections import PatchCollection
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import gridspec
import mpl_toolkits.mplot3d.art3d as art3d
from mpl_toolkits.axes_grid1 import make_axes_locatable
import os

class LixelStatistics(object):
    def __init__(self, calibration_path):
        calibration_path = os.path.abspath(calibration_path)
        self.__path2lixel_statistics = os.path.join(calibration_path, 'lixel_statistics.bin')
        self.__path2lixel_statistics_header = os.path.join(calibration_path, 'lixel_statistics.header.bin')
        self.__path2lixel_positions = os.path.join(calibration_path, 'lixel_positions.txt')
        
        lixel_statistics = np.fromfile(self.__path2lixel_statistics, dtype=np.float32) 
        lixel_statistics = lixel_statistics.reshape([lixel_statistics.shape[0]/11 ,11])

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

        self.__read_lixel_statistics_header()
        self.__read_lixel_positions_and_outer_radius()
        self.__init_lixel_polygons()

    def __read_lixel_statistics_header(self):
        lixel_statistics_header = np.fromfile(self.__path2lixel_statistics_header, dtype=np.float32)
        self.number_lixel = lixel_statistics_header[0]
        self.number_pixel = lixel_statistics_header[1]
        self.number_paxel = lixel_statistics_header[2]
        self.lixel_outer_radius = lixel_statistics_header[3]
        self.lixel_z_orientation = lixel_statistics_header[4]
        self.photons_emitted_per_lixel = lixel_statistics_header[5]

    def __read_lixel_positions_and_outer_radius(self):
        lixel_positions = np.genfromtxt(self.__path2lixel_positions)
        self.lixel_positions_x = lixel_positions[:,0]
        self.lixel_positions_y = lixel_positions[:,1]

    def __init_lixel_polygons(self):
        s32 = np.sqrt(3)/2.

        poly_template = np.array([
            [0, 1],
            [-s32, 0.5],
            [-s32, -0.5],
            [0, -1],
            [s32, -0.5],
            [s32, 0.5],
            ])
        poly_template *= self.lixel_outer_radius

        
        lixel_centers_xy = np.array([
            self.lixel_positions_x, 
            self.lixel_positions_y
        ])

        self.lixel_polygons = [xy + poly_template for xy in lixel_centers_xy.T]

def add_to_ax_symmetric_hist(vals, ax, nbins=None):
    if nbins == None:
        nbins = np.floor(np.sqrt(vals.shape[0])) 
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
def add_to_ax_2d_hist(x,y, ax, aspect='auto'):
    nbins_x = np.sqrt(x.shape[0])
    nbins_y = np.sqrt(y.shape[0])
    bins, xedges, yedges = np.histogram2d(x, y, bins=[nbins_x, nbins_y])
    im = ax.imshow(
        bins.T, 
        interpolation='none', 
        origin='low', 
        extent=[xedges[0], xedges[-1], yedges[0], yedges[-1]], 
        aspect=aspect
    )    
    im.set_cmap('viridis')
def add_to_ax_c_vs_c_std(lss, ax):
    c_mean = np.hypot(lss.cx_mean, lss.cy_mean)
    c_mean = np.rad2deg(c_mean)
    c_mean_valid = ~np.isnan(c_mean)

    c_std = np.hypot(lss.cx_std, lss.cy_std)
    c_std = np.rad2deg(c_std)
    c_std_valid = ~np.isnan(c_std)

    valid = c_mean_valid*c_std_valid
    add_to_ax_2d_hist(c_mean[valid], c_std[valid], ax)
    ax.set_xlabel('c mean/deg')
    ax.set_ylabel('c std/deg')
def add_to_ax_x_y_hist2d(lss, ax):
    valid_x = ~np.isnan(lss.x_mean)
    valid_y = ~np.isnan(lss.y_mean)
    valid = valid_x*valid_y
    x_pos = lss.x_mean[valid]
    y_pos = lss.y_mean[valid]

    add_to_ax_2d_hist(x_pos, y_pos, ax, aspect='equal')
    ax.set_xlabel('principal aperture x/m')
    ax.set_ylabel('principal aperture y/m')
def add_to_ax_cx_cy_hist2d(lss, ax):
    valid_cx = ~np.isnan(lss.cx_mean)
    valid_cy = ~np.isnan(lss.cy_mean)
    valid = valid_cx*valid_cy
    cx_pos = np.rad2deg(lss.cx_mean[valid])
    cy_pos = np.rad2deg(lss.cy_mean[valid])

    add_to_ax_2d_hist(cx_pos, cy_pos, ax, aspect='equal')
    ax.set_xlabel('cx/deg')
    ax.set_ylabel('cy/deg')    
def add_to_ax_colored_lixels(lss, I, ax, cmap='viridis', vmin=None, vmax=None):
    valid = ~np.isnan(I)
    valid_I = I[valid]
    
    valid_polygons = []
    for i, poly in enumerate(lss.lixel_polygons):
        if valid[i]:
            valid_polygons.append(poly)

    coll = PolyCollection(
        valid_polygons, 
        array=valid_I, 
        cmap=cmap,
        edgecolors='none', 
        )
    if vmin is None:
        vmin=valid_I.min()
    if vmax is None:
        vmax=valid_I.max()
    coll.set_clim([vmin, vmax])
    ax.add_collection(coll)
    ax.autoscale_view()
    ax.set_aspect('equal')
    return coll #to set colorbar

class PlotLixelStatistics(object):
    def __init__(self, lss, path):
        self.dpi = 300
        self.width = 2.0*1920.0/self.dpi
        self.hight = 2.0*1080.0/self.dpi
        self.lss = lss
        self.path = path
    def __style(self, ax):
        ax.spines['right'].set_visible(False)
        ax.spines['top'].set_visible(False)
        ax.yaxis.tick_left()
        ax.xaxis.tick_bottom()
    def fig_ax(self):
        fig = plt.figure(figsize=(self.width, self.hight))
        ax = fig.gca()
        self.__style(ax)
        return fig, ax
    def __save_fig(self, fig, filename):
        fig.savefig(os.path.join(self.path, filename), bbox_inches='tight', dpi=self.dpi)
    def save_cx_mean(self):
        fig, ax = self.fig_ax()
        add_to_ax_cx_mean_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'cx_mean.png')
        plt.close(fig)
    def save_cy_mean(self):
        fig, ax = self.fig_ax()
        add_to_ax_cy_mean_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'cy_mean.png')
        plt.close(fig)
    def save_x_mean(self):
        fig, ax = self.fig_ax()
        add_to_ax_x_mean_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'x_mean.png')
        plt.close(fig)
    def save_y_mean(self):
        fig, ax = self.fig_ax()
        add_to_ax_y_mean_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'y_mean.png')
        plt.close(fig)
    def save_cx_stddev(self):
        fig, ax = self.fig_ax()
        add_to_ax_cx_std_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'cx_stddev.png')
        plt.close(fig)
    def save_cy_stddev(self):
        fig, ax = self.fig_ax()
        add_to_ax_cy_std_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'cy_stddev.png')
        plt.close(fig)
    def save_time_mean(self):
        fig, ax = self.fig_ax()
        add_to_ax_time_mean_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'time_mean.png')
        plt.close(fig)
    def save_time_stddev(self):
        fig, ax = self.fig_ax()
        add_to_ax_time_std_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'time_stddev.png')
        plt.close(fig)
    def save_geometrical_efficiency(self):
        fig, ax = self.fig_ax()
        add_to_ax_geometric_efficieny_hist(self.lss, ax)
        ax.semilogy()
        self.__save_fig(fig, 'geometric_efficiency.png')
        plt.close(fig)
    def save_c_mean_vs_c_std(self):
        fig, ax = self.fig_ax()
        add_to_ax_c_vs_c_std(self.lss, ax)
        self.__save_fig(fig, 'c_mean_vs_c_std.png')
        plt.close(fig)
    def save_x_y_hist2d(self):
        fig, ax = self.fig_ax()
        add_to_ax_x_y_hist2d(self.lss, ax)
        self.__save_fig(fig, 'x_y_mean_hist2d.png')
        plt.close(fig)
    def save_cx_cy_hist2d(self):
        fig, ax = self.fig_ax()
        add_to_ax_cx_cy_hist2d(self.lss, ax)
        self.__save_fig(fig, 'cx_cy_mean_hist2d.png')
        plt.close(fig)
    def save_sensor_plane_overview(self, I, name='unknown', unit='unknown'):
        fig, ax = self.fig_ax()
        coll = add_to_ax_colored_lixels(self.lss, I, ax)
        ax.set_ylabel('sensor plane y/m')
        ax.set_xlabel('sensor plane x/m')
        fig.colorbar(coll, label='principal aperture '+name+'/'+unit)
        self.__save_fig(fig, 'overview_'+name+'.png')
        # zoom center
        outer_radius = 1.0/np.sqrt(2.0)*np.hypot(
            self.lss.lixel_positions_x.max(), 
            self.lss.lixel_positions_y.max()
        )
        zoom_radius = 1.0/10.0*outer_radius
        ax.set_ylim([-zoom_radius, zoom_radius])
        ax.set_xlim([-zoom_radius, zoom_radius])
        self.__save_fig(fig, 'overview_'+name+'_zoom_center.png')
        # zoom pos x
        ax.set_ylim([-zoom_radius, zoom_radius])
        ax.set_xlim([0.95*outer_radius-zoom_radius, 0.95*outer_radius+zoom_radius])
        self.__save_fig(fig, 'overview_'+name+'_zoom_pos_x.png')
        # zoom pos y
        ax.set_ylim([0.95*outer_radius-zoom_radius, 0.95*outer_radius+zoom_radius])
        ax.set_xlim([-zoom_radius, zoom_radius])
        self.__save_fig(fig, 'overview_'+name+'_zoom_pos_y.png')
        plt.close(fig)
    def save(self):
        self.save_cx_mean()
        self.save_cy_mean()
        self.save_x_mean()
        self.save_y_mean()
        self.save_cx_stddev()
        self.save_cy_stddev()
        self.save_time_mean()
        self.save_time_stddev()
        self.save_geometrical_efficiency()
        self.save_c_mean_vs_c_std()
        self.save_x_y_hist2d()
        self.save_cx_cy_hist2d()
        self.save_sensor_plane_overview(self.lss.geometric_efficiency, 'efficiency', '1')
        self.save_sensor_plane_overview(self.lss.x_mean, 'x_mean', 'm')
        self.save_sensor_plane_overview(self.lss.x_std, 'x_stddev', 'm')
        self.save_sensor_plane_overview(self.lss.y_mean, 'y_mean', 'm')
        self.save_sensor_plane_overview(self.lss.y_std, 'y_stddev', 'm')
        self.save_sensor_plane_overview(np.rad2deg(self.lss.cx_mean), 'cx_mean', 'deg')
        self.save_sensor_plane_overview(np.rad2deg(self.lss.cx_std), 'cx_stddev', 'deg')
        self.save_sensor_plane_overview(np.rad2deg(self.lss.cy_mean), 'cy_mean', 'deg')
        self.save_sensor_plane_overview(np.rad2deg(self.lss.cy_std), 'cy_stddev', 'deg')