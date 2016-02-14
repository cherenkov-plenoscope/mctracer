#include "VisualConfig.h"
//------------------------------------------------------------------------------
VisualConfig::VisualConfig():sky_dome(Color::sky_blue) {

	preview.cols = 320;
	preview.rows = 180;

	snapshot.cols = 1920;
	snapshot.rows = 1080;
	snapshot.rays_per_pixel = 3; // good trade off of computation time and image smoothness 
	snapshot.focal_length_over_aperture_diameter = 0.95; //ultra fast lens
	snapshot.image_sensor_size_along_a_row = 0.07; //IMAX 70mm format

	global_illumination.on = true;
	global_illumination.incoming_direction = Vector3D(0.6, 0.2 ,1.0);

	photon_trajectories.radius = 0.025;
}