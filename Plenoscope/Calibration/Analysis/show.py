#!/usr/bin/env python
from __future__ import absolute_import, print_function, division
import numpy as np
import pandas as pd
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
import tempfile
import pickle
from copy import deepcopy

class PlenoscopeCalibration(object):
    def __init__(self, path2lixel_statistics_condensate):
        self.__path2lixel_statistics_condensate = os.path.abspath(path2lixel_statistics_condensate)
        self.__path2lixel_positions = os.path.join(os.path.dirname(self.__path2lixel_statistics_condensate), 'lixel_positions.csv')
       
        self.lixel_statistics = np.genfromtxt(self.__path2lixel_statistics_condensate)      
        self.__read_lixel_positions_and_outer_radius(self.__path2lixel_positions)

    def __read_lixel_positions_and_outer_radius(self, path):
        self.paxel_sensor_positions = np.genfromtxt(path)
        lixel_outer_radius = 0.0
        lixel_z_orientation = 0.0
        with open(path) as f:
            for i, line in enumerate(f):
                if "lixel_outer_radius" in line:
                    lixel_outer_radius = float(line[23:].strip('\n'))

                if "lixel_z_orientation" in line:
                    lixel_z_orientation = float(line[24:].strip('\n'))

                if i > 100:
                    break

        self.lixel_outer_radius = lixel_outer_radius
        self.lixel_z_orientation = lixel_z_orientation