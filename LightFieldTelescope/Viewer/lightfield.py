import scipy.spatial
import numpy as np

class PlenoscopeLightFieldCalibration:

    def __init__(self, path):
        # currently there is only one epoch. 
        # In the future, there might be more
        # this constructor, can hopefully *guess*, what epoch
        # a calibration file refers to.
        self.load_plenoscope_calibration_epoch_160310(path)

    def load_plenoscope_calibration_epoch_160310(self, plenoscope_calib_path):
        
        raw = np.genfromtxt(plenoscope_calib_path)
        
        # geometrical_efficiency[1] cx[rad] cy[rad] x[m]    y[m]    t[s]
        self.raw = np.rec.fromarrays(
            (raw[:,0], raw[:,1], raw[:,2], raw[:,3], raw[:,4], raw[:,5]), 
            dtype=[
                ('geometrical_efficiency', 'f8'),
                ('cx_mean', 'f8'),
                ('cy_mean', 'f8'),
                ('x_mean', 'f8'),
                ('y_mean', 'f8'),
                ('time_delay_from_principal_apertur_to_sub_pixel', 'f8'),
            ]
        )

        f = open(plenoscope_calib_path)
        for i in range(100):
            line = f.readline()
            if line[0:28] == '# number_of_direction_bins: ':
                self.n_pixel = int(line[28:])

            if line[0:37] == '# number_of_principal_aperture_bins: ':
                self.n_paxel = int(line[37:])

        self.__post_init()

    def __post_init(self):
        self.n_cells = self.n_paxel*self.n_pixel
        self.paxel_pos = self.__estimate_principal_aperture_bin_positions()
        self.pixel_pos = self.__estimate_fov_direction_bin_positions()
        self.paxel_efficiency_along_all_pixel = np.nanmean(np.reshape(self.raw['geometrical_efficiency'], newshape=(self.n_pixel, self.n_paxel)), axis=0)

    def __estimate_principal_aperture_bin_positions(self):
        paxel_pos = np.zeros(shape=(2, self.n_paxel))
        for PA_bin in range(self.n_paxel):
            paxel_pos[0, PA_bin] += np.nanmean(self.raw['x_mean'][PA_bin::self.n_paxel])
            paxel_pos[1, PA_bin] += np.nanmean(self.raw['y_mean'][PA_bin::self.n_paxel])

        return np.rec.fromarrays((paxel_pos[0,:], paxel_pos[1,:]), 
            dtype=[('x', 'f8'),('y', 'f8')]
        )

    def __estimate_fov_direction_bin_positions(self):
        pixel_pos = np.zeros(shape=(2, self.n_pixel))
        for dir_bin in range(self.n_pixel):
            pixel_pos[0, dir_bin] = np.nanmean(self.raw['cx_mean'][self.n_paxel*dir_bin:self.n_paxel*(dir_bin+1)])
            pixel_pos[1, dir_bin] = np.nanmean(self.raw['cy_mean'][self.n_paxel*dir_bin:self.n_paxel*(dir_bin+1)])

        return np.rec.fromarrays((pixel_pos[0,:], pixel_pos[1,:]), 
            dtype=[('x', 'f8'),('y', 'f8')]
        )

    def bin2pap_bin(self, abin):
        return abin%n_paxel

    def bin2dir_bin(self, abin):
        return int(abin/n_paxel)

class PlenoscopeLightField:

    def __init__(self, path):
        # currently there is only one epoch. 
        # In the future, there might be more
        # this constructor, can hopefully *guess*, what epoch
        # a calibration file refers to.
        self.load_epoch_160310(path)

    def load_epoch_160310(self, path):
        raw = np.genfromtxt(path)
        self.raw = np.rec.fromarrays(
            (raw[:,0], raw[:,1]), 
            dtype=[
                ('sub_pixel_arrival_time', 'f8'),
                ('number_photons_raw', 'f8')
            ]
        )

        self.n_cells = self.raw.shape[0]

class LightField():
    
    def __init__(self, plfc, plf):

        # INTENSITY
        where_sensitive = plfc.raw['geometrical_efficiency'] != 0.

        intensity = np.zeros(shape=plfc.n_cells)
        intensity[where_sensitive] = plf.raw['number_photons_raw'][where_sensitive]/plfc.raw['geometrical_efficiency'][where_sensitive]
        intensity[np.invert(where_sensitive)] = 0

        mean_sensitivity = plfc.raw['geometrical_efficiency'].sum()/plfc.n_cells
        intensity *= mean_sensitivity

        #ARRIVAL TIMES
        arrival_times = np.zeros(shape=plf.n_cells)
        arrival_times[where_sensitive] = plf.raw['sub_pixel_arrival_time'][where_sensitive] - plfc.raw['time_delay_from_principal_apertur_to_sub_pixel'][where_sensitive]
        arrival_times -= arrival_times.min()
        
        #reshape
        self.eff = np.reshape(plfc.raw['geometrical_efficiency'], newshape=(plfc.n_pixel, plfc.n_paxel))
        self.I = np.reshape(plf.raw['number_photons_raw'], newshape=(plfc.n_pixel, plfc.n_paxel))
        #self.I = np.reshape(intensity, newshape=(plfc.n_pixel, plfc.n_paxel))
        self.t = np.reshape(arrival_times, newshape=(plfc.n_pixel, plfc.n_paxel))

        self.n_cells = plfc.n_cells
        self.n_paxel = plfc.n_paxel
        self.n_pixel = plfc.n_pixel

        self.pixel_pos = plfc.pixel_pos
        self.paxel_pos = plfc.paxel_pos

        self.pixel_pos_tree = scipy.spatial.cKDTree(np.array([self.pixel_pos["x"], self.pixel_pos["y"]]).T)
        self.paxel_pos_tree = scipy.spatial.cKDTree(np.array([self.paxel_pos["x"], self.paxel_pos["y"]]).T)
        
        #efficiency
        self.paxel_eff = plfc.paxel_efficiency_along_all_pixel


if __name__ == "__main__":
    plfc = PlenoscopeLightFieldCalibration("sub_pixel_statistics.txt")
    plf = PlenoscopeLightField("1.txt")
    lf = LightField(plfc, plf)

    # testing - stupid
    assert int(lf.I.sum()) == 389954
    assert lf.I.shape == (8431, 127)

    assert lf.eff.shape == (8431, 127)

    assert lf.n_cells == 1070737
    assert lf.n_paxel == 127
    assert lf.n_pixel == 8431

    assert lf.paxel_eff.shape == (127,)
    assert lf.paxel_pos.shape == (127,)
    assert lf.paxel_pos_tree.n == 127
    
    assert lf.pixel_pos.shape == (8431,)
    assert lf.pixel_pos_tree.n == 8431

    assert lf.t.shape == (8431, 127)