"""propagate.

Usage:
  propagate.py <geometry> <corsika_file>

Options:
  -h --help      Show this screen.
  --version      Show version.
  --output_path  output_path [default=.]

"""

from docopt import docopt
import pytracer as T
from eventio import EventIoFileStream
import numpy as np 
from math import sqrt
import matplotlib.pyplot as plt

arguments = docopt(__doc__, version='propagate 0.1')
print(arguments)

prng = T.Mt19937(T.zero_seed)
speed_of_light = 299792458
production_distance_offset = 1e3

def calc_causal_directions(event_bunches):
    """returns (N, 3) array of causal_directions"""
    b = event_bunches
    N = len(b)

    d = np.zeros((N, 3))
    d[:, 0] = b['cx']
    d[:, 1] = b['cy']
    d[:, 2] = np.sqrt(1. - d[:, 0]**2 - d[:, 1]**2)

    return d


def calc_causal_supports(event_bunches, causal_directions):
    b = event_bunches
    N = len(b)

    s = np.zeros((N, 3))
    s[:, 0] = b['x']/100.
    s[:, 1] = b['y']/100.

    rp = b['time']*1e-9 * speed_of_light + production_distance_offset

    s += rp[:, np.newaxis] * causal_directions

    return s

def make_photons(event_bunches, causal_supports, causal_directions):
    photons = []
    b = event_bunches
    N = len(b)
    wavelength = (np.abs(b['lambda'])*1e-9).astype(np.float64)
    for i in range(N):
        cherenkov_photon = T.Photon(
            T.Vector3D(*causal_supports[i]),
            T.Vector3D(*(-causal_directions[i])),
            wavelength[i])
        cherenkov_photon.set_id(i)
        photons.append(cherenkov_photon)

    return photons

settings = T.TracerSettings()
settings.SetMultiThread(False)
settings.store_only_last_intersection(True)

fab = T.WorldFactory()
fab.load(arguments['<geometry>'])
world = fab.world()

array_ctrl = fab.get_telescope_array_control()

sensors = fab.sensors_in_world()

corsika_run = EventIoFileStream(arguments['<corsika_file>'])


for event_id, event in enumerate(corsika_run):

    zd = float(corsika_run.current_event_header["angle in radian: (zenith, azimuth)"][0])
    az = float(corsika_run.current_event_header["angle in radian: (zenith, azimuth)"][0])
    array_ctrl.move_all_to_Az_Zd(az, zd)

    env = T.PropagationEnvironment()
    env.world_geometry = world
    env.propagation_options = settings
    env.random_engine = prng

    
    #bar = progressbar.ProgressBar()

    # absorb photons
    random_numbers = np.array([prng.uniform() for x in range(len(event.bunches))])
    print "before absorption: ", len(event.bunches)
    event.bunches = event.bunches[random_numbers <= event.bunches['photons']]
    print "after absorption: ", len(event.bunches)

    causal_directions = calc_causal_directions(event.bunches)
    causal_supports = calc_causal_supports(event.bunches, causal_directions)

    photons = make_photons(event.bunches, causal_supports, causal_directions)
    
    for photon in photons:
        photon.propagate_in(env)

    T.reset_all_sesnors(sensors)
    
    for photon in photons:
        T.assign_photon_to_sensors(photon, sensors)    
    
    my_dtype=[
        ('x_intersect', 'f8'), 
        ('y_intersect', 'f8'), 
        ('theta_x', 'f8'), 
        ('theta_y', 'f8'), 
        ('arrival_times_since_emission', 'f8'), 
        ('id', 'i4'), 
        ('production_height_over_sea_level', 'f8'), 
        ]
    sensor_arrival_tables = [
         np.array(list(sensor.get_arrival_table()), dtype=my_dtype) for sensor in sensors
        ]
    
    for i, p in enumerate(photons):
        p.delete_history()
    del photons


    plt.close('all')
    fig, axes = plt.subplots(1,2)
    for ax, at,s  in zip(axes, sensor_arrival_tables, sensors):
        x_name = 'x_intersect'
        y_name = 'y_intersect' 
        _range = [
            [at[x_name].min(),at[x_name].max()],
            [at[y_name].min(),at[y_name].max()],
            ]
        ax.hist2d(at[x_name], at[y_name], 
            bins=[200, 200], 
            range=_range, 
            cmap="Greys")
        ax.set_title(s.get_frame().get_path_in_tree_of_frames())
        ax.set_xlabel(x_name)
        ax.set_ylabel(y_name)
        ax.grid()
    
    
    plt.savefig("{:04d}.png".format(event_id))
