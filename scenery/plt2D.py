#!/usr/bin/python
from __future__ import print_function
import numpy as np
import matplotlib.pyplot as plt
import os
import sys

def main():
	filename = sys.argv[1]
	csv_file = open(filename)
	photons = np.loadtxt(filter(lambda row: row[0]!='#', csv_file))
	csv_file.close()

	roi = float(sys.argv[2])

	num_of_bins = np.sqrt(photons.shape[0])
	print("num_of_bins: ", num_of_bins)
	#print("x ", photons[:,0], " y ", photons[:,1])
	print("min x ", np.min(photons[:,0]), " y ", np.min(photons[:,1]))
	print("max x ", np.max(photons[:,0]), " y ", np.max(photons[:,1]))
	print("mean x ", np.mean(photons[:,0]), " y ", np.mean(photons[:,1]))
	print("std dev x ", np.std(photons[:,0]), "std dev y ", np.std(photons[:,1]))

	H, xedges, yedges = np.histogram2d(
		photons[:,0],
		photons[:,1], 
		bins=(num_of_bins, num_of_bins),
	)


	my_dpi = 180
	hight = 8

	fig = plt.figure(figsize=(hight, hight), dpi=my_dpi)

	plt.rcParams.update({'font.size': 12})

	plt.rc('text', usetex=True)
	plt.rc('font', family='serif')
	plt.axis([-roi,roi,-roi,roi])
	plt.xlabel('x [m]')
	plt.ylabel('y [m]')
	X, Y = np.meshgrid(xedges, yedges)
	ax = fig.gca()
	ax.pcolor(Y, X, np.sqrt(H.T), cmap='gray_r')
	ax.set_aspect('equal')
	#plt.show()
	fig.savefig(
		os.path.splitext(filename)[0]+'.png',
		bbox_inches='tight', 
		figsize={1.5,1.5}, 
		dpi=my_dpi
	)
	plt.close(fig)

if __name__ == "__main__":
    main()


