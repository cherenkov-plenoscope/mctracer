import os
import subprocess as sp
import numpy as np
from io import BytesIO
from skimage import io


def read_ppm_image(fstream):
    magic = fstream.readline()
    assert magic[0] == 80
    assert magic[1] == 54
    comment = fstream.readline()
    image_size = fstream.readline()
    number_columns, number_rows = image_size.decode().split()
    number_columns = int(number_columns)
    number_rows = int(number_rows)
    max_color = int(fstream.readline().decode())
    assert max_color == 255
    count = number_columns*number_rows*3
    raw = np.fromstring(fstream.read(count), dtype=np.uint8)
    img = raw.reshape((number_rows, number_columns, 3))
    return img


def cam_command(
    position=[0, 0, 2],
    orientation=[0, 0, 0],
    object_distance=10,
    sensor_size=0.06,
    field_of_view=np.deg2rad(45),
    f_stop=0.95,
    number_columns=512,
    number_rows=288,
    noise_level=25
):
    fout = BytesIO()
    fout.write(np.uint64(645).tobytes())  # MAGIC

    fout.write(np.float64(position[0]).tobytes())  # position
    fout.write(np.float64(position[1]).tobytes())
    fout.write(np.float64(position[2]).tobytes())

    fout.write(np.float64(orientation[0]).tobytes())  # Tait–Bryan-angles
    fout.write(np.float64(orientation[1]).tobytes())
    fout.write(np.float64(orientation[2]).tobytes())

    fout.write(np.float64(object_distance).tobytes())
    fout.write(np.float64(sensor_size).tobytes())  # sensor-size
    fout.write(np.float64(field_of_view).tobytes())  # fov
    fout.write(np.float64(f_stop).tobytes())  # f-stop

    fout.write(np.uint64(number_columns).tobytes())
    fout.write(np.uint64(number_rows).tobytes())
    fout.write(np.uint64(noise_level).tobytes())

    fout.seek(0)
    return fout.read()

"""
call = [path_exe, '--scenery', scenery_path, '--config', visual_path]
merlict = sp.Popen(call, stdin=sp.PIPE, stdout=sp.PIPE)
outdir = 'fly4'
os.makedirs(outdir, exist_ok=True)

for i, y in enumerate(np.linspace(-10, -1, 100)):
    w = merlict.stdin.write(
        cam_command(
            position=[0, 0, y],
            orientation=[0, np.deg2rad(-90), 0],
            object_distance=np.abs(y)))
    w = merlict.stdin.flush()
    img = read_ppm_image(merlict.stdout)
    io.imsave(os.path.join(outdir, '{:06d}.tiff'.format(i)), img)
"""