
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.collections import RegularPolyCollection, PolyCollection
from matplotlib import collections, transforms
from matplotlib.colors import colorConverter
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
from matplotlib.axes import Axes
import os
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib import cm
import matplotlib.cm

plt.ticklabel_format(style='sci', axis='x', scilimits=(-2,3))
plt.FHD16to9_fig_size = {9.6, 5.4}
plt.FHD16to9_dpi = 400
plt.rcParams.update({'font.size': 12})

overview_file_path = "overview.txt"

def get_subpixel_outer_radius_from_overview_file(path):
    lines = open(path).readlines()
    sub_pixel_info_line_id = None
    for line_id, line in enumerate(lines):
        if "Sub_Pixel__" in line:
            sub_pixel_info_line_id = line_id + 1

    if not sub_pixel_info_line_id is None:
        for line in lines[sub_pixel_info_line_id:]:
            if "outer radius" in line:
                num_str = line.split()[-1]
                return float(num_str[:-1])



def get_offsets():
    return np.genfromtxt('sub_pixel_positions.csv')

global_offsets = get_offsets()


def offsets_to_verts(offsets, radius=7.7/1000.):

    s32 = np.sqrt(3)/2.

    #verts = np.array([
    #    [1 ,0],
    #    [0.5, s32],
    #    [-0.5, s32],
    #    [-1, 0],
    #    [-0.5, -s32],
    #    [0.5, -s32],
    #    ])

    verts = np.array([
        [0, 1],
        [-s32, 0.5],
        [-s32, -0.5],
        [0, -1],
        [s32, -0.5],
        [s32, 0.5],
        ])


    verts *= radius

    
    return [off + verts for off in offsets]

global_vertices = offsets_to_verts(
    offsets=global_offsets,
    radius=get_subpixel_outer_radius_from_overview_file(
        overview_file_path)
    )

def camera(self,
           data,
           cmap='hot',
           vmin=None,
           vmax=None,
           colorbar=True,
           ):

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

Axes.camera = camera

def pltcamera(*args, **kwargs):
    ax = plt.gca()
    ret = ax.camera(*args, **kwargs)
    plt.draw_if_interactive()
    return ret

plt.camera = pltcamera

