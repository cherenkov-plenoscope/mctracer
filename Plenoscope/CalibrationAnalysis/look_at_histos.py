from __future__ import print_function
import numpy as np
import matplotlib.pyplot as plt
import plot
import pandas as pd

store = pd.HDFStore('lftc_analysis.h5', mode='r')

photon_count = store['photon_count']

data = np.zeros(len(plot.global_offsets), dtype=np.float32)
data[store.photon_count.index.values] = store.photon_count.values

number_of_photon_count_bins = np.sqrt(len(photon_count))
if number_of_photon_count_bins > np.max(photon_count):
    number_of_photon_count_bins = np.max(photon_count)

plt.figure(figsize = plt.FHD16to9_fig_size)
plt.hist(data, bins=number_of_photon_count_bins, log=True, histtype='step')
plt.title("photon_count")
plt.savefig("histo_plots/photon_count.png", dpi=plt.FHD16to9_dpi)
plt.close('all')

data_names = ['x', 'y', 'cx', 'cy', 'time']
analysis_names = ['means', 'stds', 'medians']

for analysis_name in analysis_names:
    for data_name in data_names:
        series = store[analysis_name][data_name]

        data = np.zeros(len(plot.global_offsets), dtype=np.float32)
        data[series.index.values] = series.values

        plt.figure(figsize = plt.FHD16to9_fig_size)
        plt.hist(data, bins=200, log=True, histtype='step')
        plt.title(data_name+"  "+analysis_name)
        plt.savefig(
        	"histo_plots/"+data_name+"_"+analysis_name+".png",
    		dpi=plt.FHD16to9_dpi
    		)
        plt.close('all')

        print(data_name, analysis_name)

#### zoom
analysis_name = 'stds'
data_name = 'time'
series = store[analysis_name][data_name]

data = np.zeros(len(plot.global_offsets), dtype=np.float32)
data[series.index.values] = series.values

plt.figure(figsize = plt.FHD16to9_fig_size)
plt.hist(data, bins=500,range=[0, 5e-9], log=True, histtype='step')
plt.title(data_name+"  "+analysis_name)
plt.savefig(
	"histo_plots/"+data_name+"_"+analysis_name+"__zoom.png",
    dpi=plt.FHD16to9_dpi
    )
plt.close('all')
print(data_name, analysis_name)

#### only non zero, i.e. there is a valid mean
analysis_name = 'means'
data_name = 'time'
series = store[analysis_name][data_name]

data = np.zeros(len(plot.global_offsets), dtype=np.float32)
data[series.index.values] = series.values
data = data[np.nonzero(data)[0]]

plt.figure(figsize = plt.FHD16to9_fig_size)
plt.hist(data, bins=500, log=True, histtype='step')
plt.title(data_name+"  "+analysis_name)
plt.savefig(
    "histo_plots/"+data_name+"_"+analysis_name+"__drop_zero_means.png",
    dpi=plt.FHD16to9_dpi
    )
plt.close('all')
print(data_name, analysis_name)

store.close()
