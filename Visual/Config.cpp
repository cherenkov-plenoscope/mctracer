#include "Config.h"

namespace Visual {
//------------------------------------------------------------------------------
Config::Config():sky_dome(Color::sky_blue) {
	max_interactions = 42;

	preview.cols = 128;
	preview.rows = 72;
	preview.scale = 10;

	snapshot.cols = 1920;
	snapshot.rows = 1080;
	snapshot.rays_per_pixel = 3; // good trade off of computation time and image smoothness 
	snapshot.focal_length_over_aperture_diameter = 0.95; //ultra fast lens
	snapshot.image_sensor_size_along_a_row = 0.07; //IMAX 70mm format

	global_illumination.on = true;
	global_illumination.incoming_direction = Vec3(0.6, 0.2 ,1.0);

	photon_trajectories.radius = 0.025;
}
}//Visual