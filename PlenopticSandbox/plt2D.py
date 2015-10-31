#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

def main():
	filename = sys.argv[1]

	csv_file = open(filename)
	photons = np.loadtxt(filter(lambda row: row[0]!='#', csv_file))
	csv_file.close()

	num_of_bins = np.sqrt(photons.shape[0])
	print("num_of_bins: ", num_of_bins)

	xwidth = photons[:,0].max() - photons[:,0].min()
	ywidth = photons[:,1].max() - photons[:,1].min()

	xcen = (photons[:,0].max() + photons[:,0].min())/2.0
	ycen = (photons[:,1].max() + photons[:,1].min())/2.0

	lwidth = 1.0
	if xwidth > ywidth:
		lwidth = xwidth
	else:
		lwidth = ywidth

	xmin = xcen - lwidth/2.0
	xmax = xcen + lwidth/2.0
	ymin = ycen - lwidth/2.0
	ymax = ycen + lwidth/2.0

	H, xedges, yedges = np.histogram2d(
		photons[:,0], photons[:,1],
		bins=(num_of_bins, num_of_bins),
		range=[[xmin,xmax],[ymin,ymax]]
	)

	X, Y = np.meshgrid(xedges, yedges)

	overhead = 1.05

	fig = plt.figure()
	plt.rcParams.update({'font.size': 12})
	plt.rc('text', usetex=True)
	plt.rc('font', family='serif')
	plt.xlabel('x [m]')
	plt.ylabel('y [m]')
	plt.axis((xmin*overhead, xmax*overhead, ymin*overhead, ymax*overhead))
	ax = fig.gca()
	ax.pcolormesh(X, Y, np.sqrt(H), cmap='Greys')
	ax.set_aspect('equal', adjustable='box')
	plt.show()

	fig.savefig(
		os.path.splitext(filename)[0]+'.png', 
		bbox_inches='tight', 
		figsize={1.5,1.5}, 
		dpi=400
	)

if __name__ == "__main__":
    main()