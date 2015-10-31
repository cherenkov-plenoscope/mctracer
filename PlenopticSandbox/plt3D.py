#!/usr/bin/python
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import os
import sys

def main():
	filename = sys.argv[1]
	csv_file = open(filename)
	photons = np.loadtxt(filter(lambda row: row[0]!='#', csv_file))
	csv_file.close()

	fig = plt.figure()
	ax = fig.gca(projection='3d')
	ax.set_xlabel('x[m]')
	ax.set_ylabel('y[m]')
	ax.set_zlabel('z[m]')
	c0 = 299792458.0

	# shifting relative arrival times
	photons[:,4] = photons[:,4] - photons[:,4].min() 

	backtrace_time = 0.0e-9

	# limit number of photons
	count = 0
	for photon in photons:
		count = count + 1
		if count > 100:
			break

		# support on principal aperture plane
		support = np.array([photon[0], photon[1], 0.0])

		# directions w.r.t optical axis
		cos_theta_x = photon[2]
		cos_theta_y = photon[3]

		direction = np.array([
			cos_theta_x, 
			cos_theta_y, 
			np.sqrt(1.0 - (cos_theta_x**2.0 + cos_theta_y**2.0))
		])

		direction = direction
		relative_arrival_time = photon[4]

		production = support + direction*c0*(relative_arrival_time + backtrace_time)

		# ray
		x = [support[0],production[0]]
		y = [support[1],production[1]]
		z = [support[2],production[2]]

		# photon itself
		px = production[0]
		py = production[1]
		pz = production[2]

		ax.plot(x, y, z)
		ax.scatter(px, py, pz, c='b', marker='o')

	plt.show()

	fig.savefig(
		os.path.splitext(filename)[0]+'.png', 
		bbox_inches='tight', 
		figsize={1.5,1.5}, 
		dpi=400
	)

if __name__ == "__main__":
    main()