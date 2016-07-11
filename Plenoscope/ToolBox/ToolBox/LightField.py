#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
from Image import Image
from ApertureMasks import circular_mask

__all__ = ['LightField']
    
class LightField(object):
    def __init__(self, raw_plenoscope_response, lixel_statistics, sensor_plane2imaging_system, correct_intensity=False):
        self.lixel_statistics = lixel_statistics
        self.sensor_plane2imaging_system = sensor_plane2imaging_system
        
        # INTENSITY
        self.I = raw_plenoscope_response.intensity.copy()
        self.I = self.I.reshape(
            self.lixel_statistics.number_pixel, 
            self.lixel_statistics.number_paxel)
        # correct for efficiency of lixels
        where_sensitive = self.lixel_statistics.mask_of_fraction_of_most_efficient_lixels(0.95)
        if correct_intensity:
            mean_efficiency_where_sensitive = self.lixel_statistics.efficiency[where_sensitive].mean()
            self.I[np.invert(where_sensitive)] = 0.0
            self.I[where_sensitive] /= self.lixel_statistics.efficiency[where_sensitive]
            self.I[where_sensitive] *= mean_efficiency_where_sensitive

        # ARRIVAL TIMES
        self.t = raw_plenoscope_response.arrival_time.copy()
        self.t = self.t.reshape(
            self.lixel_statistics.number_pixel, 
            self.lixel_statistics.number_paxel)
        # correct for time delay of lixels
        self.t -= self.lixel_statistics.time_delay_mean
        self.t[np.invert(where_sensitive)] = 0.0
        self.t -= self.t.min()

    def get_alpha(self, wanted_object_distance):
        focal_length = self.lixel_statistics.expected_focal_length_of_imaging_system
        image_sensor_distance = self.sensor_plane2imaging_system.light_filed_sensor_distance

        alpha = 1/image_sensor_distance * 1/((1/focal_length) - (1/wanted_object_distance))
        return alpha

    def refocus(self, wanted_object_distance):
        focal_length = self.lixel_statistics.expected_focal_length_of_imaging_system
        alpha = self.get_alpha(wanted_object_distance)

        n_pixel = self.lixel_statistics.number_pixel
        n_paxel = self.lixel_statistics.number_paxel
        
        dx = np.tan(self.lixel_statistics.pixel_pos_cx)*focal_length
        dy = np.tan(self.lixel_statistics.pixel_pos_cy)*focal_length

        px = self.lixel_statistics.paxel_pos_x
        py = self.lixel_statistics.paxel_pos_y

        dx_tick = px[:, np.newaxis] + (dx[np.newaxis, :] - px[:, np.newaxis])/alpha
        dy_tick = py[:, np.newaxis] + (dy[np.newaxis, :] - py[:, np.newaxis])/alpha

        dx_des = np.arctan(dx_tick/focal_length)
        dy_des = np.arctan(dy_tick/focal_length)

        # (127*8400, 2)
        desired_x_and_y = np.zeros((np.prod(dx_des.shape), 2))
        desired_x_and_y[:, 0] = dx_des.flatten()
        desired_x_and_y[:, 1] = dy_des.flatten()

        nearest_pixel_distances, nearest_pixel_indices = self.lixel_statistics.pixel_pos_tree.query(desired_x_and_y)

        summanden = self.I[nearest_pixel_indices, np.arange(n_paxel).repeat(n_pixel)]
        summanden = summanden.reshape(n_paxel, n_pixel)
        
        return Image(
            summanden.sum(axis=0), 
            self.lixel_statistics.pixel_pos_cx, 
            self.lixel_statistics.pixel_pos_cy)

    def __zero_mask_save_average(self, I, axis, mask):
        if mask.sum() == 0.0:
            return np.zeros(I.shape[np.invert(axis)])
        else:
            return np.average(I, axis=axis, weights=mask)     

    def pixel_sum(self, mask=None):
        return Image(
            self.__zero_mask_save_average(self.I, axis=1, mask=mask), 
            self.lixel_statistics.pixel_pos_cx, 
            self.lixel_statistics.pixel_pos_cy)

    def paxel_sum(self, mask=None):
        return Image(
            self.__zero_mask_save_average(self.I, axis=0, mask=mask), 
            self.lixel_statistics.paxel_pos_x, 
            self.lixel_statistics.paxel_pos_y)

    def pixel_sum_sub_aperture(self, sub_x, sub_y, sub_r, smear=0.0):
        """
        Returns an Image as seen by a sub aperture at sub_x, sub_y with radius sub_r.
        The subaperture radius sub_r can be smeared.
        """
        return self.pixel_sum(
            circular_mask(
                self.lixel_statistics.paxel_pos_x,
                self.lixel_statistics.paxel_pos_y,
                x=sub_x,
                y=sub_y,
                r=sub_r,
                smear=smear
            )
        )
