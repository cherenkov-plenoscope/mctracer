import scipy.spatial
import numpy as np

class PlenoscopeLightFieldCalibration:

    def __init__(self, path):
        # currently there is only one epoch.
        # In the future, there might be more
        # this constructor, can hopefully *guess*, what epoch
        # a calibration file refers to.
        self.load_plenoscope_calibration_epoch_160310(path)

    def load_plenoscope_calibration_epoch_160310(self, path):
        self.n_pixel, self.n_paxel = self._find_npixel_npaxel(path)

        raw = np.genfromtxt(path, unpack=True)
        for i, attribute_name in enumerate([
                'geometrical_efficiency',
                'cx_mean',
                'cy_mean',
                'x_mean',
                'y_mean',
                'time_delay_from_principal_apertur_to_sub_pixel',
                ]):
            setattr(self, attribute_name, raw[i].reshape(self.n_pixel, self.n_paxel))

        self.paxel_pos = np.rec.array([
                np.nanmean(self.x_mean, axis=0), 
                np.nanmean(self.y_mean, axis=0),
            ], 
            dtype=[('x', 'f8'), ('y', 'f8')])

        self.pixel_pos = np.rec.array([
                np.nanmean(self.cx_mean, axis=1), 
                np.nanmean(self.cy_mean, axis=1),
            ], 
            dtype=[('x', 'f8'), ('y', 'f8')])

        self.paxel_efficiency_along_all_pixel = np.nanmean(self.geometrical_efficiency, axis=0)

    def _find_npixel_npaxel(self, path):
        f = open(path)
        for i in range(100):
            line = f.readline()
            if line[0:28] == '# number_of_direction_bins: ':
                n_pixel = int(line[28:])

            if line[0:37] == '# number_of_principal_aperture_bins: ':
                n_paxel = int(line[37:])
        return n_pixel, n_paxel


class LightField:

    def __init__(self, path, calibration=None):
        # currently there is only one epoch.
        # In the future, there might be more
        # this constructor, can hopefully *guess*, what epoch
        # a calibration file refers to.
        self.load_epoch_160310(path)

        if calibration is not None:
            self.calibrate(calibration)

    def load_epoch_160310(self, path):
        self._raw = np.genfromtxt(path, unpack=True)


    def calibrate(self, plfc):
        self.plfc = plfc
        self.n_paxel = plfc.n_paxel
        self.n_pixel = plfc.n_pixel

        for i, attribute_name in enumerate([
                'sub_pixel_arrival_time',
                'number_photons_raw',
                ]):
            setattr(self, attribute_name, self._raw[i].reshape(self.n_pixel, self.n_paxel))

        where_sensitive = plfc.geometrical_efficiency != 0.

        # INTENSITY - not used
        # intensity = self.number_photons_raw.copy()
        # intensity /= plfc.geometrical_efficiency / plfc.geometrical_efficiency.mean()
        # intensity[np.invert(where_sensitive)] = 0
        
        # ARRIVAL TIMES
        self.t = self.sub_pixel_arrival_time.copy()
        self.t -= plfc.time_delay_from_principal_apertur_to_sub_pixel
        self.t[np.invert(where_sensitive)] = 0
        self.t -= self.t.min()

        self.eff = plfc.geometrical_efficiency
        self.I = self.number_photons_raw

        self.pixel_pos = plfc.pixel_pos
        self.paxel_pos = plfc.paxel_pos

        self.pixel_pos_tree = scipy.spatial.cKDTree(np.array([self.pixel_pos.x, self.pixel_pos.y]).T)
        self.paxel_pos_tree = scipy.spatial.cKDTree(np.array([self.paxel_pos.x, self.paxel_pos.y]).T)

        # efficiency
        self.paxel_eff = plfc.paxel_efficiency_along_all_pixel


if __name__ == "__main__":
    plfc = PlenoscopeLightFieldCalibration("sub_pixel_statistics.txt")
    lf = LightField("1.txt", plfc)

    # testing - stupid
    assert int(lf.I.sum()) == 389954
    assert lf.I.shape == (8431, 127)

    assert lf.eff.shape == (8431, 127)

    assert lf.n_paxel == 127
    assert lf.n_pixel == 8431

    assert lf.paxel_eff.shape == (127,)
    assert lf.paxel_pos.shape == (127,)
    assert lf.paxel_pos_tree.n == 127

    assert lf.pixel_pos.shape == (8431,)
    assert lf.pixel_pos_tree.n == 8431

    assert lf.t.shape == (8431, 127)