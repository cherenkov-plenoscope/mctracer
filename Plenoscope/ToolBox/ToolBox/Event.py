#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import glob, os
from SensorPlane2ImagingSystem import SensorPlane2ImagingSystem
from RawPlenoscopeResponse import RawPlenoscopeResponse
from McTruth import McTruth
from LightField import LightField

__all__ = ['Event']


class Event(object):
    def __init__(self, path, lixel_statistics):
        self.__path = os.path.abspath(path)
        
        self.raw_plenoscope_response = RawPlenoscopeResponse(
            os.path.join(self.__path, 'raw_plenoscope_response.bin')
        )

        self.sensor_plane2imaging_system = SensorPlane2ImagingSystem(
            os.path.join(self.__path, 'sensor_plane2imaging_system.bin')
        )

        self.mc_truth = McTruth(
            os.path.join(self.__path, 'mc_truth')
        )

        self.light_field = LightField(
            self.raw_plenoscope_response,
            lixel_statistics,
            self.sensor_plane2imaging_system
        )

    def __str__(self):
        out = '_Event_'+str(int(self.mc_truth.corsika_event_header[1]))+'_\n'
        out+= ' path: '+self.__path+'\n'
        out+= '  '+self.mc_truth.__str__().replace('\n', '\n  ')
        return out
        
    def __repr__(self):
        return self.__str__()