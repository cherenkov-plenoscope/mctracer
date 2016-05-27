#!/usr/bin/env python
"""
Plot ligth field calibration statistics

Usage: viwe -i=RAW_DIRNAME -o=OUTPUT_PATH

Options:
    -i --input=RAW_DIRNAME      dirname where mct wrote the calibration binary
    -o --output=OUTPUT_PATH     where to save the plots and the condensed hdf5
"""
from __future__ import absolute_import, print_function, division
import docopt
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import pandas as pd
import os

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
from .CalibrationCondensate import CalibrationCondensate

def main():
    try:

        # DEFINE PATHS
        #-------------
        arguments = docopt.docopt(__doc__)

        out_patht = os.path.abspath(arguments['--output'])
        in_path = os.path.abspath(arguments['--input'])

        c = CalibrationCondensate(in_path)

        # Monkey Patch PyPlot
        #--------------------
        print('Monkey patch pyplot')
        plt.ticklabel_format(style='sci', axis='x', scilimits=(-2,3))
        plt.FHD16to9_fig_size = {9.6, 5.4}
        plt.FHD16to9_dpi = 400
        plt.rc('text', usetex=True)
        plt.rc('font', family='serif')
        plt.rcParams.update({'font.size': 8})

        def camera(self, data, cmap='hot', vmin=None, vmax=None, colorbar=True,):

            coll = PolyCollection(
                c.sub_pixel_vertices, 
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

        
        # Look
        #-----
        print('count')
        photon_count = c.sub_pixel_stats['photon_count']

        data = np.zeros(len(c.paxel_sensor_positions), dtype=np.float32)
        data[c.sub_pixel_stats.photon_count.index.values] = c.sub_pixel_stats.photon_count.values

        fig, ax = plt.subplots(figsize=plt.FHD16to9_fig_size)
        ax.spines['right'].set_visible(False)
        ax.spines['top'].set_visible(False)
        ax.yaxis.tick_left()
        ax.xaxis.tick_bottom()
        ax.set_xlim([xmin,xmax])
        ax.set_ylim([ymin,ymax])
        img = plt.camera(data, cmap='viridis')
        fig.colorbar(img)
        plt.tight_layout()
        fig.savefig(
            os.path.join(out_patht, "photon_count.png"), 
            dpi=plt.FHD16to9_dpi
            )
        plt.close(fig)

        """
        data_names = ['x', 'y', 'cx', 'cy', 'time']
        analysis_names = ['means', 'stds', 'medians']

        for analysis_name in analysis_names:
            df = c.sub_pixel_stats[analysis_name]
            for data_name in data_names:
                series = df[data_name]

                data = np.zeros(len(c.paxel_sensor_positions), dtype=np.float32)
                data[series.index.values] = series.values

                plt.figure(figsize = plt.FHD16to9_fig_size)
                img = plt.camera(data)
                plt.colorbar(img)
                plt.title(data_name+"  "+analysis_name)
                plt.xlabel('x[m]')
                plt.ylabel('y[m]')
                plt.axis('equal')
                plt.savefig(
                    os.path.join(out_patht, data_name+"_"+analysis_name+".png"),
                    dpi=plt.FHD16to9_dpi
                )
                plt.close('all')
                print(data_name, analysis_name)
        """
    except docopt.DocoptExit as e:
        print(e)

if __name__ == "__main__":
    main()