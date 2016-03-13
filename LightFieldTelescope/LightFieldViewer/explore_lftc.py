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

	def aperture(self, subpix_id, subpix=0):
		if subpix==0:
			subpix = self.data[self.data['id']==subpix_id]


		print("mean x ", np.mean(subpix['x'])," y ", np.mean(subpix['y']))
		print("stddev x ", np.std(subpix['x'])," y ", np.std(subpix['y']))
		plt.figure()
		plt.hist2d(
			subpix['x'], 
			subpix['y'], 
			range=self.aperture_range, 
			bins=100, 
			cmap='hot'
		)
		plt.xlabel("x [m]")
		plt.ylabel("y [m]")
		plt.show()

	def direction(self, subpix_id, subpix=0):
		if subpix==0:
			subpix = self.data[self.data['id']==subpix_id]

		print("mean x ", np.mean( np.rad2deg( np.arcsin(subpix['cx']) ) )," y ", np.mean( np.rad2deg( np.arcsin(subpix['cy']) ) ) )
		print("stddev x ", np.std( np.rad2deg( np.arcsin(subpix['cx']) ) )," y ", np.std( np.rad2deg( np.arcsin(subpix['cy']) ) ) )

		plt.figure()
		plt.hist2d(
			np.rad2deg(np.arcsin(subpix['cx'])),
			np.rad2deg(np.arcsin(subpix['cy'])), 
			range=self.direction_range,
			bins=1000,
			cmap='hot'
		)
		plt.xlabel("x direction [deg]")
		plt.ylabel("y direction [deg]")
		plt.show()

	def time(self, subpix_id, subpix=0, my_bin_count=None):
		if subpix==0:
			subpix = self.data[self.data['id']==subpix_id]

		if subpix.shape[0]==0:
			return

		print("time std dev ", np.std(subpix['t']))

		if my_bin_count is None:
			my_bin_count = 2.0*np.sqrt(subpix['t'].shape[0])
			
		plt.figure()
		plt.hist(subpix['t'], bins=my_bin_count, histtype='step')
		plt.xlabel("t [s]")
		plt.show()

	def all(self, subpix_id):
		subpix = self.data[self.data['id']==subpix_id]
		print("photon count ",str(subpix.shape[0]))
		self.aperture(subpix_id, subpix)
		self.direction(subpix_id, subpix)
		self.time(subpix_id, subpix)

