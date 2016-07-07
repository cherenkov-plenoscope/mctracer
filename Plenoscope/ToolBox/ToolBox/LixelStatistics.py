#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import scipy.spatial
import os

__all__ = ['LixelStatistics']
    
class LixelStatistics(object):
    def __init__(self, path):
        path = os.path.abspath(path)

        self.__read_light_field_sensor_geometry_header(os.path.join(path, 'light_field_sensor_geometry.header.bin'))
        self.__read_lixel_positions(os.path.join(path, 'lixel_positions.bin'))
        self.__read_lixel_statistics(os.path.join(path, 'lixel_statistics.bin'))

        self.__calc_pixel_and_paxel_average_positions()
        self.__init_lixel_polygons()

    def __calc_pixel_and_paxel_average_positions(self):
        self.paxel_pos_x = np.nanmean(self.x_mean, axis=0) 
        self.paxel_pos_y = np.nanmean(self.y_mean, axis=0)

        self.pixel_pos_cx = np.nanmean(self.cx_mean, axis=1)
        self.pixel_pos_cy = np.nanmean(self.cy_mean, axis=1)

        self.pixel_pos_tree = scipy.spatial.cKDTree(
            np.array([self.pixel_pos_cx, self.pixel_pos_cy]).T)
        self.paxel_pos_tree = scipy.spatial.cKDTree(
            np.array([self.paxel_pos_x, self.paxel_pos_y]).T)

        self.paxel_efficiency_along_pixel = np.nanmean(self.efficiency, axis=0)
        self.pixel_efficiency_along_paxel = np.nanmean(self.efficiency, axis=1)

    def __read_lixel_statistics(self, path):
        ls = np.fromfile(path, dtype=np.float32) 
        ls = ls.reshape([ls.shape[0]/11 ,11])

        for i, attribute_name in enumerate([
                'efficiency',
                'cx_mean','cx_std',
                'cy_mean','cy_std',
                'x_mean','x_std',
                'y_mean','y_std',
                'time_mean','time_std'
                ]):
            setattr(
                self, 
                attribute_name, 
                ls[:,i].reshape(self.number_pixel, self.number_paxel)
            )        

    def __read_light_field_sensor_geometry_header(self, path):
        gh = np.fromfile(path, dtype=np.float32)
        self.number_pixel = gh[101-1]
        self.number_paxel = gh[102-1]
        self.number_lixel = self.number_pixel*self.number_paxel
        
        self.lixel_outer_radius = gh[103-1]
        self.lixel_z_orientation = gh[105-1]

        self.sensor_plane2imaging_system = np.array([
            [ gh[ 11-1], gh[ 14-1], gh[ 17-1], gh[ 20-1] ],
            [ gh[ 12-1], gh[ 15-1], gh[ 18-1], gh[ 21-1] ],
            [ gh[ 13-1], gh[ 16-1], gh[ 19-1], gh[ 22-1] ],
        ])

        self.light_field_sensor_distance = self.sensor_plane2imaging_system[2,3]

    def __read_lixel_positions(self, path):
        lp = np.fromfile(path, dtype=np.float32)
        lp = lp.reshape([lp.shape[0]/2 ,2])
        self.lixel_positions_x = lp[:,0]
        self.lixel_positions_y = lp[:,1]

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