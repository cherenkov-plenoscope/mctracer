from __future__ import print_function
import numpy as np
import matplotlib.pyplot as plt
import plot
import pandas as pd

store = pd.HDFStore('lftc_analysis.h5', mode='r')

photon_count = store['photon_count']

data = np.zeros(len(plot.global_offsets), dtype=np.float32)
data[store.photon_count.index.values] = store.photon_count.values

plt.figure(figsize = plt.FHD16to9_fig_size)
img = plt.camera(data)
plt.title("photon_count")
plt.xlim(3.6, 4.2)
plt.ylim(-0.3, 0.3)

plt.colorbar(img)
plt.savefig("plots_zoomright/photon_count.png", dpi=plt.FHD16to9_dpi)
plt.close('all')

data_names = ['x', 'y', 'cx', 'cy', 'time']
analysis_names = ['means', 'stds', 'medians']

for analysis_name in analysis_names:
    df = store[analysis_name]
    for data_name in data_names:
        series = df[data_name]

        data = np.zeros(len(plot.global_offsets), dtype=np.float32)
        data[series.index.values] = series.values

        plt.figure(figsize = plt.FHD16to9_fig_size)
        if data_name=='time' and analysis_name=='stds':
            img = plt.camera(data, vmax=8e-9)
        else:
            img = plt.camera(data)
        plt.xlim(3.6, 4.2)
        plt.ylim(-0.3, 0.3)
        plt.colorbar(img)
        plt.title(data_name+"  "+analysis_name)
        plt.savefig("plots_zoomright/"+data_name+"_"+analysis_name+".png", dpi=plt.FHD16to9_dpi)
        plt.close('all')

        print(data_name, analysis_name)

store.close()