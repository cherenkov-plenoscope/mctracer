#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
import os
import sys

def main():
	filename = sys.argv[1]
	csv_file = open(filename)
	photons = np.loadtxt(filter(lambda row: row[0]!='#', csv_file))
	csv_file.close()

	num_of_bins = 10*np.sqrt(photons.shape[0])
	print("num_of_bins: ", num_of_bins)
	print("x ", photons[:,0], " y ", photons[:,1])
	print("mean x ", np.mean(photons[:,0]), " y ", np.mean(photons[:,1]))
	print("std dev x ", np.std(photons[:,0]), "std dev y ", np.std(photons[:,1]))

	H, xedges, yedges = np.histogram2d(photons[:,0], photons[:,1], bins=(num_of_bins, num_of_bins))

	H = np.sqrt(H)

	fig2 = plt.figure()

	plt.rcParams.update({'font.size': 12})

	plt.rc('text', usetex=True)
	plt.rc('font', family='serif')
	plt.xlabel('x [m]')
	plt.ylabel('y [m]')
	X, Y = np.meshgrid(xedges, yedges)
	ax = fig2.gca()
	ax.pcolormesh(X, Y, H.T, cmap='Greys')
	ax.set_aspect('equal')
	plt.show()
	fig2.savefig(os.path.splitext(filename)[0]+'.png', bbox_inches='tight', figsize={1.5,1.5}, dpi=400)

if __name__ == "__main__":
    main()