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

