//=================================
// include guard
#ifndef __CAMERAMANFORSTEREO3D_H_INCLUDE__
#define __CAMERAMANFORSTEREO3D_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "DistanceMeter.h"
#include "CameraDevice.h"
//=================================
class CameraManForStereo3D {
public:
	CameraManForStereo3D(CameraDevice* camera_to_work_with);
	void aquire_stereo_image(	
		const Frame* world,
		const GlobalSettings* settings
	);
	void increase_stereo_offset();
	void decrease_stereo_offset();
	const CameraImage* get_anaglyph_stereo3D_image();
	void use_same_stereo_offset_as(const CameraManForStereo3D *college);
private:
	CameraDevice* camera;

	CameraImage *left_image;
	CameraImage *right_image;

	Vector3D initial_camera_pos;
	Rotation3D initial_camera_rotation;
	Vector3D initial_camera_image_upward_direction;

	Vector3D left_camera_pos;
	Rotation3D left_camera_rotation;
	Vector3D left_camera_direction_optical_axis;

	Vector3D right_camera_pos;
	Rotation3D right_camera_rotation;
	Vector3D right_camera_direction_optical_axis;

	double distance_to_object;
	Vector3D intersection_point_for_l_and_r_optical_axes;

	double stereo_offset_in_m = 6e-2;
	void calc_pos_and_rot_for_left_and_right_camera_config(	
		const Frame* world,
		const GlobalSettings* settings
	);
	void remember_initial_camera_config();
	void set_up_camera_back_to_initial_config();
	void remember_initial_camera_position_and_rotation();
	void remmber_initial_camera_image_upward_direction();
	void set_positions_for_left_and_right_stereo_config();
	void set_object_distance_to_focus_on(
		const Frame* world,
		const GlobalSettings* settings
	);
	void set_intersec_point_for_left_and_right_optical_axis();
	void set_pointing_dir_for_left_and_right_stereo_config();
	void set_up_camera_in_left_stereo_config();
	void set_up_camera_in_right_stereo_config();
	Vector3D offset_to_the_right()const;
	void set_intersection_for_l_and_r_cameras_optical_axes();
	void print_stereo_offset_manipulation(const std::string status)const;
	void take_left_image(	
		const Frame* world,
		const GlobalSettings* settings
	);
	void take_right_image(	
		const Frame* world,
		const GlobalSettings* settings
	);
};
#endif // __CAMERAMANFORSTEREO3D_H_INCLUDE__