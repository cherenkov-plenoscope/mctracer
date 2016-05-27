from __future__ import absolute_import, print_function, division
import numpy as np
import os
import pandas as pd
from matplotlib.colors import LogNorm
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.collections import RegularPolyCollection, PolyCollection
from matplotlib import collections, transforms
from matplotlib.colors import colorConverter
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
from matplotlib.axes import Axes
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib import cm
import matplotlib.cm

class PlotSubPixels(object):

    print('Monkey patch pyplot')
    plt.ticklabel_format(style='sci', axis='x', scilimits=(-2,3))
    plt.FHD16to9_fig_size = {9.6, 5.4}
    plt.FHD16to9_dpi = 400
    plt.rcParams.update({'font.size': 12})

    global_vertices = sub_pixel_vertices(sub_pixel_pos_xy, paxel_sensor_outer_radius)

    def camera(self, data, cmap='hot', vmin=None, vmax=None, colorbar=True,):

        verts = global_vertices

        coll = PolyCollection(
            verts, 
            array=data, 
            cmap=cmap,
            edgecolors='none', 
            )

        if vmin is None:
            vmin=data.min()
        if vmax is None:
            vmax=data.max()

        coll.set_clim([vmin, vmax])
        
        self.add_collection(coll)
        self.autoscale_view()
        self.set_aspect('equal')

        return coll

    def pltcamera(*args, **kwargs):
        ax = plt.gca()
        ret = ax.camera(*args, **kwargs)
        plt.draw_if_interactive()
        return ret

    Axes.camera = camera
    plt.camera = pltcamera
