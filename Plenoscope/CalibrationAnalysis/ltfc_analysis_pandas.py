import numpy as np
import pandas as pd

light_field_calib_binary_file = "the_big_lebowsky.lftc"
light_field_calib_hd5_output = 'lftc_analysis.h5'

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
df = pd.DataFrame( np.fromfile(
    light_field_calib_binary_file, 
    dtype=lftc_dtype, 
    count=-1))

# Sort the Light Field Calibration based on the subpixel ids
grouped_by_id = df.groupby('id')

# export a condensed Light Field Calibration file
store = pd.HDFStore(light_field_calib_hd5_output)
store["means"] = grouped_by_id.mean().dropna()
store["medians"] = grouped_by_id.median().dropna()
store["stds"] = grouped_by_id.std().dropna()
store["photon_count"] = df['id'].value_counts()
store.close()