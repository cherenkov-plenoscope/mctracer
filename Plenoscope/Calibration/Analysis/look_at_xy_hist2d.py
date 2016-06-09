import numpy as np
import matplotlib.pyplot as plt

lftc_dtype = np.dtype([
    ('id', np.int32),
    ('x', np.float32),
    ('y', np.float32),
    ('cx', np.float32),
    ('cy', np.float32),
    ('time', np.float32),
    ])

data = np.fromfile(
    "/home/relleums/the_big_lebowsky.lftc", 
    dtype=lftc_dtype, 
    count=-1)

plt.hist2d(data['x'], data['y'], bins=200, cmap='hot')
plt.xlabel("x-position on principal aperture [m]")
plt.ylabel("y-position on principal aperture [m]")
plt.colorbar()
plt.savefig('plots/x_y_hist2d.png', dpi=600)
plt.close('all')

plt.hist2d(np.rad2deg(data['cx']), np.rad2deg(data['cy']), bins=200, cmap='hot')
plt.xlabel("incoming x-direction [deg]")
plt.ylabel("incoming y-direction [deg]")
plt.colorbar()
plt.savefig('plots/cx_cy_hist2d.png')
