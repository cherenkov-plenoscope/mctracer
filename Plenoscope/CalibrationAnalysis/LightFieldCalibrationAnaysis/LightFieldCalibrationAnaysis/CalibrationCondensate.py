from __future__ import absolute_import, print_function, division
import numpy as np
import os
import pandas as pd

class CalibrationCondensate(object):

    def __init__(self, calbration_dirname):
        self.__read_epoch_160310(calbration_dirname)

    def __read_epoch_160310(self, calbration_dirname):

        calbration_dirname = os.path.abspath(calbration_dirname)
        raw_photon_file = os.path.join(calbration_dirname, 'the_big_lebowsky.lftc')
        sub_pix_pos_file = os.path.join(calbration_dirname, 'paxel_sensor_positions.txt')
        pd_photon_file = os.path.join(calbration_dirname, 'statistics.h5')

        self.__read_sub_pixel_positions_and_outer_radius(sub_pix_pos_file)
            
        self.__set_up_sub_pixel_vertices()

        self.__read_binary2pandas(
            binary_calibration_input=raw_photon_file, 
            condensed_hdf5_output=pd_photon_file
        )

    def __set_up_sub_pixel_vertices(self):

        verts = np.zeros(shape=(6,2))
        for i in range(6):
            vert_angle = self.sub_pixel_orientation + i*(2.0*np.pi/6)
            verts[i,:] = [np.cos(vert_angle), np.sin(vert_angle)]
        verts *= self.paxel_sensor_outer_radius
        self.sub_pixel_vertices = [pos + verts for pos in self.paxel_sensor_positions]

    def __read_sub_pixel_positions_and_outer_radius(self, path):
        self.paxel_sensor_positions = np.genfromtxt(path)
        paxel_sensor_outer_radius = 0.0
        sub_pixel_orientation = 0.0
        with open(path) as f:
            for i, line in enumerate(f):
                if "paxel_sensor_outer_radius" in line:
                    paxel_sensor_outer_radius = float(line[27:].strip('\n'))

                if "paxel_sensor_hexagon_z_orientation" in line:
                    sub_pixel_orientation = float(line[28:].strip('\n'))

                if i > 100:
                    break

        self.paxel_sensor_outer_radius = paxel_sensor_outer_radius
        self.sub_pixel_orientation = sub_pixel_orientation

    def __read_binary2pandas(self, binary_calibration_input, condensed_hdf5_output):
        # Light Field Calibration binary format
        lftc_dtype = np.dtype([
            ('id', np.int32),
            ('x', np.float32),
            ('y', np.float32),
            ('cx', np.float32),
            ('cy', np.float32),
            ('time', np.float32),
        ])

        # read in binary Light Field Calibration into Panda Data Frame
        df = pd.DataFrame( 
            np.fromfile(
                binary_calibration_input, 
                dtype=lftc_dtype, 
                count=-1
            )
        )

        # Sort the Light Field Calibration based on the subpixel ids
        grouped_by_id = df.groupby('id')

        # export a condensed Light Field Calibration file
        self.sub_pixel_stats = pd.HDFStore(condensed_hdf5_output)
        self.sub_pixel_stats["means"] = grouped_by_id.mean().dropna()
        self.sub_pixel_stats["medians"] = grouped_by_id.median().dropna()
        self.sub_pixel_stats["stds"] = grouped_by_id.std().dropna()
        self.sub_pixel_stats["photon_count"] = df['id'].value_counts()
