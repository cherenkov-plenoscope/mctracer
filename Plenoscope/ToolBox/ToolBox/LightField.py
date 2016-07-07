#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
from Image import Image

__all__ = ['LightField']
    
class LightField(object):
    def __init__(self, raw_plenoscope_response, calib, sensor_plane2imaging_system):
        self.calib = calib
        where_sensitive = self.calib.efficiency != 0.0

        # INTENSITY
        self.I = raw_plenoscope_response.intensity.copy()
        self.I = self.I.reshape(self.calib.number_pixel, self.calib.number_paxel)
        
        # ARRIVAL TIMES
        self.t = raw_plenoscope_response.arrival_time.copy()
        self.t = self.t.reshape(self.calib.number_pixel, self.calib.number_paxel)
        self.t -= self.calib.time_delay_mean
        self.t[np.invert(where_sensitive)] = 0.0
        self.t -= self.t.min()

    def get_alpha(self, wanted_object_distance):
        focal_length = self.calib.expected_focal_length_of_imaging_system
        image_sensor_distance = self.calib.sensor_plane2imaging_system.light_filed_sensor_distance

        alpha = 1/image_sensor_distance * 1/((1/focal_length) - (1/wanted_object_distance))
        return alpha

    def refocus(self, wanted_object_distance):
        focal_length = self.calib.expected_focal_length_of_imaging_system
        alpha = self.get_alpha(wanted_object_distance)

        n_pixel = self.calib.number_pixel
        n_paxel = self.calib.number_paxel
        
        dx = np.tan(self.calib.pixel_pos_cx)*focal_length
        dy = np.tan(self.calib.pixel_pos_cy)*focal_length

        px = self.calib.paxel_pos_x
        py = self.calib.paxel_pos_y

        dx_tick = px[:, np.newaxis] + (dx[np.newaxis, :] - px[:, np.newaxis])/alpha
        dy_tick = py[:, np.newaxis] + (dy[np.newaxis, :] - py[:, np.newaxis])/alpha

        dx_des = np.arctan(dx_tick/focal_length)
        dy_des = np.arctan(dy_tick/focal_length)

        # (127*8400, 2)
        desired_x_and_y = np.zeros((np.prod(dx_des.shape), 2))
        desired_x_and_y[:, 0] = dx_des.flatten()
        desired_x_and_y[:, 1] = dy_des.flatten()

        nearest_pixel_distances, nearest_pixel_indices = self.calib.pixel_pos_tree.query(desired_x_and_y)

        summanden = self.I[nearest_pixel_indices, np.arange(n_paxel).repeat(n_pixel)]
        summanden = summanden.reshape(n_paxel, n_pixel)
        
        return Image(summanden.sum(axis=0), self.calib.pixel_pos_cx, self.calib.pixel_pos_cy)

    def pixel_sum(self, mask=None):
        return Image(np.sum(self.I, axis=1), self.calib.pixel_pos_cx, self.calib.pixel_pos_cy)

    def paxel_sum(self, mask=None):
        return Image(np.sum(self.I, axis=0), self.calib.paxel_pos_x, self.calib.paxel_pos_y)
