import os
import subprocess
import numpy as np
import io
import skimage
from skimage import io as skimage_io
import tempfile


def read_ppm_image(fstream):
    magic = fstream.readline()
    assert magic[0] == 80
    assert magic[1] == 54
    comment = fstream.readline()
    image_size = fstream.readline()
    num_columns, num_rows = image_size.decode().split()
    num_columns = int(num_columns)
    num_rows = int(num_rows)
    max_color = int(fstream.readline().decode())
    assert max_color == 255
    count = num_columns * num_rows * 3
    raw = np.frombuffer(fstream.read(count), dtype=np.uint8)
    img = raw.reshape((num_rows, num_columns, 3))
    return img


def camera_command(
    position=[0, 0, 2],
    orientation=[0, 0, 0],
    object_distance=10,
    sensor_size=0.06,
    field_of_view=np.deg2rad(45),
    f_stop=0.95,
    num_columns=512,
    num_rows=288,
    noise_level=25,
):
    """
    Returns a binary command-string to be fed into merlict's cmaera-server via
    std-in.

    Parameters
    ----------

        noise_level             From 0 (no noise) to 255 (strong noise).
                                Lower noise_levels give better looking images
                                but take longer to be computed. Strong
                                noise_levels are fast to compute.
    """
    fout = io.BytesIO()
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

    fout.write(np.uint64(num_columns).tobytes())
    fout.write(np.uint64(num_rows).tobytes())
    fout.write(np.uint64(noise_level).tobytes())

    fout.seek(0)
    return fout.read()


class CameraServer:
    def __init__(
        self, merlict_camera_server_path, scenery_path, visual_config_path,
    ):
        self.call = [
            merlict_camera_server_path,
            "--scenery",
            scenery_path,
            "--config",
            visual_config_path,
        ]
        self.server = subprocess.Popen(
            self.call, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
        )

    def render_image(self, image_config):
        _ = self.server.stdin.write(camera_command(**image_config))
        _ = self.server.stdin.flush()
        image = read_ppm_image(fstream=self.server.stdout)
        return image

    def render_image_and_write_to_tiff(self, image_config, path):
        assert os.path.splitext(path)[1] == ".tiff"
        image = self.render_image(image_config=image_config)
        skimage.io.imsave(path, image)

    def __exit__(self):
        self.server.terminate()
