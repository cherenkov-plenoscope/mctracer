import os
import subprocess as sp
import numpy as np
from io import BytesIO
from skimage import io

path_exe = '/home/sebastian/raytracing/build/mctCameraServer'
visual_path = '/home/sebastian/raytracing/mctracer/Main/Examples/Show/visual.xml'
scenery_path = '/home/sebastian/raytracing/mctracer/Main/Examples/Show/fact.xml'


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
    b = img[:, :, 0].copy()
    img[: , :, 0] = img[: , :, 2]
    img[: , :, 2] = b
    return img


def cam_command(
    position=[0, 0, 2],
    orientation=[0, 0, 0],
    object_distance=10,
    sensor_size=0.06,
    field_of_view=np.deg2rad(45),
    f_stop=0.95,
    number_columns=1920,
    number_rows=1080,
    number_rays_per_pixel=1
):
    fout = BytesIO()
    fout.write(np.uint64(645).tobytes())  # MAGIC

    fout.write(np.float64(position[0]).tobytes())  # position
    fout.write(np.float64(position[1]).tobytes())
    fout.write(np.float64(position[2]).tobytes())

    fout.write(np.float64(orientation[0]).tobytes())  # Euler-angles
    fout.write(np.float64(orientation[1]).tobytes())
    fout.write(np.float64(orientation[2]).tobytes())

    fout.write(np.float64(object_distance).tobytes())
    fout.write(np.float64(sensor_size).tobytes())  # sensor-size
    fout.write(np.float64(field_of_view).tobytes())  # fov
    fout.write(np.float64(f_stop).tobytes())  # f-stop

    fout.write(np.uint64(number_columns).tobytes())
    fout.write(np.uint64(number_rows).tobytes())
    fout.write(np.uint64(number_rays_per_pixel).tobytes())

    fout.seek(0)
    return fout.read()


call = [path_exe, '--scenery', scenery_path, '--config', visual_path]
mctracer = sp.Popen(call, stdin=sp.PIPE, stdout=sp.PIPE)
outdir = 'fly4'
os.makedirs(outdir, exist_ok=True)

for i, y in enumerate(np.linspace(-10, -1, 100)):
    w = mctracer.stdin.write(
        cam_command(
            position=[0, 0, y],
            orientation=[0, np.deg2rad(-90), 0],
            object_distance=np.abs(y)))
    w = mctracer.stdin.flush()
    img = read_ppm_image(mctracer.stdout)
    io.imsave(os.path.join(outdir, '{:06d}.tiff'.format(i)), img)