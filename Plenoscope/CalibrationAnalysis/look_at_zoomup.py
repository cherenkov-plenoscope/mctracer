from __future__ import print_function
import numpy as np
import matplotlib.pyplot as plt
import plot
import pandas as pd

store = pd.HDFStore('lftc_analysis.h5', mode='r')

photon_count = store['photon_count']

data = np.zeros(len(plot.global_offsets), dtype=np.float32)
data[store.photon_count.index.values] = store.photon_count.values

img = plt.camera(data)
plt.title("photon_count")
plt.xlim(-0.3, 0.3)
plt.ylim(3.6, 4.2)

plt.colorbar(img)
plt.savefig("plots_zoomup/photon_count.png")
plt.close('all')

data_names = ['x', 'y', 'cx', 'cy', 'time']
analysis_names = ['means', 'stds', 'medians']

for analysis_name in analysis_names:
    df = store[analysis_name]
    for data_name in data_names:
        series = df[data_name]

        data = np.zeros(len(plot.global_offsets), dtype=np.float32)
        data[series.index.values] = series.values

        if data_name=='time' and analysis_name=='stds':
            img = plt.camera(data, vmax=8e-9)
        else:
            img = plt.camera(data)
        plt.xlim(-0.3, 0.3)
        plt.ylim(3.6, 4.2)
        plt.colorbar(img)
        plt.xlabel('x[m]')
        plt.ylabel('y[m]')
        plt.axis('equal')
        plt.title(data_name+"  "+analysis_name)
        plt.savefig("plots_zoomup/"+data_name+"_"+analysis_name+".png")
        plt.close('all')

        print(data_name, analysis_name)


