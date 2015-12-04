from __future__ import print_function
import matplotlib.pyplot as plt
import numpy as np

class LightFieldTelescopeExplorer(object):

	def __init__(self, filename):
		self.mydtype = np.dtype(
	    [
	        ('id', np.uint32),
	        ('x',  np.float32),
	        ('y',  np.float32),
	        ('cx', np.float32),
	        ('cy', np.float32),
	        ('t',  np.float32),
	    ])

		self.data = np.fromfile(filename, dtype=self.mydtype)
		print("read in full file")

		self.aperture_range = np.array([ (-28.,28.), (-28.,28.) ])
		self.direction_range = np.array([ (-4.,4.), (-4.,4.) ])

	def aperture(self, subpix_id, subpix=None):
		if subpix==None:
			subpix = self.data[self.data['id']==subpix_id]

		plt.figure()
		plt.hist2d(subpix['x'], subpix['y'], range=self.aperture_range, bins=100, cmap='hot')
		plt.xlabel("x [m]")
		plt.ylabel("y [m]")
		plt.show()

	def direction(self, subpix_id, subpix=None):
		if subpix==None:
			subpix = self.data[self.data['id']==subpix_id]

		plt.figure()
		plt.hist2d(np.rad2deg(np.arcsin(subpix['cx'])), np.rad2deg(np.arcsin(subpix['cy'])), range=self.direction_range, bins=1000, cmap='hot')
		plt.xlabel("x direction [deg]")
		plt.ylabel("y direction [deg]")
		plt.show()

	def time(self, subpix_id, subpix=None):
		if subpix==None:
			subpix = self.data[self.data['id']==subpix_id]

		if subpix.shape[0]==0:
			return

		plt.figure()
		plt.hist(subpix['t'], bins=100, histtype='step')
		plt.xlabel("t [s]")
		plt.show()

	def all(self, subpix_id):
		subpix = self.data[self.data['id']==subpix_id]
		self.aperture(subpix_id, subpix)
		self.direction(subpix_id, subpix)
		self.time(subpix_id, subpix)

