//=================================
// include guard
#ifndef __APERTURECAMERA_H_INCLUDED__
#define __APERTURECAMERA_H_INCLUDED__

//=================================
// forward declared dependencies
class Frame;
//=================================
// included dependencies
#include "Core/DistanceMeter.h"
#include "CameraDevice.h"
#include "Core/Random/Random.h"
//=================================

namespace Visual {

class ApertureCamera :public CameraDevice{
public:
	using CameraDevice::CameraDevice;

	void set_focus_to(const double ObjectDistance_in_m);

	void set_FoV_in_rad(const double field_of_view);

	void set_fStop_sesnorWidth_rayPerPixel(	
		const double new_FStopNumber,
		const double new_SensorSizeX,
		const unsigned int rays_per_pixel 
	);
	
	CameraRay get_ray_for_pixel_in_row_and_col(const unsigned int row, const unsigned int col);

	void acquire_image(
		const Frame* world,
		const Config* visual_config
	);

	void auto_focus(const Frame* world);

	std::string str()const;
private:
	double FStopNumber;			
	double ApertureRadius_in_m;	
	double FocalLength_in_m; 	
	double ObjectDistance_in_m; 
	double SensorDistance_in_m; 
	double sensor_width_in_m; 	
	double sensor_height_in_m; 	
	double PixelPitch_in_m; 
	const double default_object_distance_in_m = 25.0;	

	int rays_per_pixel;
	
	Random::Mt19937 prng;

	void set_F_stop_number(const double new_FStopNumber);
	void set_sensor_size_using_width(const double sensor_width_in_m);
	void update_sensor_pixel_pitch();
	void update_sensor_distance_given_focal_and_object_distance();
	void set_object_distance(const double ObjectDistance_in_m);
	void set_number_of_rays_per_pixel(const unsigned int rays_per_pixel);
	Vec3 get_random_point_on_bounded_aperture_plane();

	Vec3 get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(
		const unsigned int row, const unsigned int col
	);

	Vec3 camera_ray_support_vector_in_world_frame(
		const Vec3 &cam_ray_support_in_cam_frame
	)const;

	Vec3 camera_ray_direction_vector_in_world_frame(
		const Vec3 &cam_ray_direction
	)const;

	double get_object_size_for_image_size(const double image_size_in_m)const;
	void update_focal_length();
	void update_aperture_radius();
	void set_default_object_distance();
	std::string get_aperture_camera_print()const;
	
	unsigned int get_random_row();
	unsigned int get_random_col();

	double get_average_object_distance(const Frame* world);
	unsigned int _5_permil_of_pixels()const;
};

}//Visual

#endif // __APERTURECAMERA_H_INCLUDED__