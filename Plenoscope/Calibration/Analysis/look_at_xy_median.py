from __future__ import print_function
import matplotlib.pyplot as plt
import pandas as pd

store = pd.HDFStore('lftc_analysis.h5', mode='r')

analysis_types = ['means', 'medians']
for analysis_type in analysis_types:

	x_median = store[analysis_type]['x'].values
	y_median = store[analysis_type]['y'].values
	nbins = x_median.shape[0]**0.5
	plt.hist2d(x_median, y_median, bins=nbins, cmap='hot')
	plt.xlabel('x[m]')
	plt.ylabel('y[m]')
	plt.axis('equal')
	plt.savefig("plots/xy_"+analysis_type+".png")