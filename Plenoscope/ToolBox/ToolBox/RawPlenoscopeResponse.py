#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np

__all__ = ['RawPlenoscopeResponse']

class RawPlenoscopeResponse(object):
    def __init__(self, path):
        raw = np.fromfile(path, dtype=np.float32)
        raw = raw.reshape([raw.shape[0]/2 ,2])
        self.arrival_time = raw[:,0]
        self.intensity = raw[:,1]

    def __str__(self):
        out = 'RawPlenoscopeResponse( '
        out+= str(self.arrival_time.shape[0])+' lixel )\n'
        return out

    def __repr__(self):
        return self.__str__()