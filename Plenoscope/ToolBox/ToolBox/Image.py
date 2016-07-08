#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import matplotlib.pyplot as plt
from PlotTools import add_to_ax

__all__ = ['Image']
    
class Image:
    """
    LightField returns images when one asks for:

        * refocuse
        * pixel_sum
        * paxel_sum
    """
    def __init__(self, intensity, positions_x, positions_y):
        self.intensity = intensity
        self.pixel_pos_x = positions_x
        self.pixel_pos_y = positions_y

    def plot(self):
        fig, ax = plt.subplots()
        add_to_ax(ax, self.intensity, self.pixel_pos_x, self.pixel_pos_y)
        plt.show()

