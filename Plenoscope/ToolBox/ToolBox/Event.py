#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import glob, os
from SensorPlane2ImagingSystem import SensorPlane2ImagingSystem
from RawPlenoscopeResponse import RawPlenoscopeResponse
from McTruth import McTruth
from LightField import LightField
from PlotTools import add_pixel_image_to_ax
from PlotTools import add_paxel_image_to_ax
import matplotlib.pyplot as plt

__all__ = ['Event']


class Event(object):
    """
    A Plenoscope event
    """
    def __init__(self, path, lixel_statistics):
        self.__path = os.path.abspath(path)
        
        self.raw_plenoscope_response = RawPlenoscopeResponse(
            os.path.join(self.__path, 'raw_plenoscope_response.bin'))

        self.sensor_plane2imaging_system = SensorPlane2ImagingSystem(
            os.path.join(self.__path, 'sensor_plane2imaging_system.bin'))

        self.mc_truth = McTruth(
            os.path.join(self.__path, 'mc_truth'))

        self.light_field = LightField(
            self.raw_plenoscope_response,
            lixel_statistics,
            self.sensor_plane2imaging_system)

    def __str__(self):
        out = '_Event_'+str(int(self.mc_truth.corsika_event_header[1]))+'_\n'
        out+= ' path: '+self.__path+'\n'
        out+= '  '+self.mc_truth.__str__().replace('\n', '\n  ')
        return out
        
    def __repr__(self):
        return self.__str__()

    def plot(self):
        plt.ion()
        
        fig, axs = plt.subplots(2,2)
        plt.suptitle(self.mc_truth.short_event_info())
        add_pixel_image_to_ax(self.light_field.pixel_sum(), axs[0][0])
        add_paxel_image_to_ax(self.light_field.paxel_sum(), axs[0][1])
        self.light_field.add_arrival_time_histogram_to_ax(axs[1][0])

        max_intensity = self.raw_plenoscope_response.intensity.max()
        bins, bin_edges = np.histogram(
            self.raw_plenoscope_response.intensity,
            bins=max_intensity)

        axs[1][1].set_yscale("log")
        axs[1][1].step(bin_edges[:-1], bins)
        axs[1][1].set_xlabel('p.e. in lixel #/1')
        axs[1][1].set_ylabel('number of lixels #/1')







