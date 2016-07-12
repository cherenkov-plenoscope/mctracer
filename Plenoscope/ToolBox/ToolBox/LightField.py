#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
from Image import Image
from ApertureMasks import circular_mask

__all__ = ['LightField']
    
class LightField(object):
    def __init__(self, raw_plenoscope_response, lixel_statistics, sensor_plane2imaging_system, correct_for_efficiency=True):
        self.lixel_statistics = lixel_statistics
        self.sensor_plane2imaging_system = sensor_plane2imaging_system
        
        self.__init_intensity(raw_plenoscope_response, correct_for_efficiency)
        self.__init_arrival_times(raw_plenoscope_response)
        self.__init_valid_lixel_mask()

    def __init_intensity(self, raw_plenoscope_response, correct_for_efficiency=True):
        self.intensity = raw_plenoscope_response.intensity.copy()
        self.intensity = self.intensity.reshape(
            self.lixel_statistics.number_pixel, 
            self.lixel_statistics.number_paxel)
        # correct for efficiency of lixels
        if correct_for_efficiency:
            mean_efficiency_where_sensitive = self.lixel_statistics.efficiency[self.lixel_statistics.valid_efficiency].mean()
            self.intensity[np.invert(self.lixel_statistics.valid_efficiency)] = 0.0
            self.intensity[self.lixel_statistics.valid_efficiency] /= self.lixel_statistics.efficiency[self.lixel_statistics.valid_efficiency]
            self.intensity[self.lixel_statistics.valid_efficiency] /= mean_efficiency_where_sensitive        

    def __init_arrival_times(self, raw_plenoscope_response):
        self.arrival_time = raw_plenoscope_response.arrival_time.copy()
        self.arrival_time = self.arrival_time.reshape(
            self.lixel_statistics.number_pixel, 
            self.lixel_statistics.number_paxel)
        # correct for time delay of lixels
        self.arrival_time -= self.lixel_statistics.time_delay_mean
        self.arrival_time -= self.arrival_time[self.lixel_statistics.valid_efficiency].min()
        self.arrival_time[np.invert(self.lixel_statistics.valid_efficiency)] = 0.0        

    def __init_valid_lixel_mask(self):
        # too low efficiency
        valid_effi = self.lixel_statistics.valid_efficiency
        # too large time delay
        speed_of_light = 3e8
        max_arrival_time_without_multiple_reflections_on_imaging_system = self.lixel_statistics.expected_focal_length_of_imaging_system/speed_of_light
        valid_time = self.arrival_time < max_arrival_time_without_multiple_reflections_on_imaging_system
        self.valid_lixel = np.logical_and(valid_time, valid_effi)


    def __refocus_alpha(self, wanted_object_distance):
        focal_length = self.lixel_statistics.expected_focal_length_of_imaging_system
        image_sensor_distance = self.sensor_plane2imaging_system.light_filed_sensor_distance

        alpha = 1/image_sensor_distance * 1/((1/focal_length) - (1/wanted_object_distance))
        return alpha

    def refocus(self, wanted_object_distance):
        """
        Return directional image Intensity(cx, cy) as seen by a classical 
        Imaging Atmospheric Cherenkov Telescope when focusing on the 
        wanted_object_distance

        Parameters
        ----------
        wanted_object_distance  The wanted object distance to focus on

        Comments
        --------
        The image returned is the intesity distribution of the light rays on a 
        classic image sensor when this image sensor would have been positioned
        in the image distance b, corresponding to the wanted_object_distance g, 
        given the focal length f on the imaging system.

        1/f = 1/g + 1/b

        """
        focal_length = self.lixel_statistics.expected_focal_length_of_imaging_system
        alpha = self.__refocus_alpha(wanted_object_distance)

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

        summanden = self.intensity[nearest_pixel_indices, np.arange(n_paxel).repeat(n_pixel)]
        summanden = summanden.reshape(n_paxel, n_pixel)
        
        return Image(
            summanden.sum(axis=0), 
            self.lixel_statistics.pixel_pos_cx, 
            self.lixel_statistics.pixel_pos_cy)

    def pixel_sum(self, paxel_weights=None):
        """
        Return the directional image, i.e. Intensity(cx,cy).

        Parameters
        ----------
        paxel_weights   1D array with the weights of the paxel to be used to
                        form the image. All paxel weights =1.0 is the full
                        aperture of the plenoscope. You can provide 
                        paxel_weights which correspond to sub apertures to 
                        get images seen by only this sub aperture part.

        Comment
        -------
            This corresponds to the classical image as seen by an Imaging 
            Atmospheric Cherenkov Telescope
        """
        if paxel_weights is None:
            paxel_weights=np.ones(self.lixel_statistics.number_paxel)
        return Image(
            np.dot(self.intensity, paxel_weights), 
            self.lixel_statistics.pixel_pos_cx, 
            self.lixel_statistics.pixel_pos_cy)

    def paxel_sum(self, pixel_weights=None):
        """
        Return the positional intesity distribution on the principal aperture 
        plane, i.e. Intensity(x,y).

        Parameters
        ----------
        pixel_weights   1D array with the weights of the pixel to be used to
                        form the image. All pixel weights =1.0 is the full
                        field of view of the plenoscope. You can provide 
                        pixel_weights which correspond to sub field of views 
                        to get intensity distributions on the principal
                        aperture with only specific incoming directions 
                        cx,cy
        """
        if pixel_weights is None:
            pixel_weights=np.ones(self.lixel_statistics.number_pixel)
        return Image(
            np.dot(self.intensity.T ,pixel_weights), 
            self.lixel_statistics.paxel_pos_x, 
            self.lixel_statistics.paxel_pos_y)

    def pixel_sum_sub_aperture(self, sub_x, sub_y, sub_r, smear=0.0):
        """
        Returns an Image as seen by a sub aperture at sub_x, sub_y with radius sub_r.
        The subaperture radius sub_r can be smeared.

        Parameters
        ----------
        sub_x   x position of sub aperture on principal aperture plane

        sub_y   y position of sub aperture on principal aperture plane

        sub_r   radius of sub aperture

        smear   [optional] same units as sub_r. If smear != 0.0, paxel will
                be taken into account not abrupt but smootly according to 
                their distance to the circular sub aperture's center
                
                    /\ paxel weight
                     |
                1.0 -|-------\ 
                     |       .\ 
                     |       . \ 
                     |       .  \ 
                     |       .   \ 
                     |       .    \ 
                     |       .     \ 
                     |       .      \ 
                     |       .       \ 
                0.0 -|-------|--------|--------------->
                        sub_r     sub_r+smear       distance of paxel
                                                   to sub aperture center
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

    def add_arrival_time_histogram_to_ax(self, ax):
        bins, bin_edges = np.histogram(
            self.arrival_time[self.valid_lixel], 
            weights=self.intensity[self.valid_lixel],
            bins=int(np.ceil(0.5*np.sqrt(self.valid_lixel.sum()))))

        ax.step(bin_edges[:-1], bins)
        ax.set_xlabel('arrival time t/s')
        ax.set_ylabel('p.e. #/1')

        return bins, bin_edges
