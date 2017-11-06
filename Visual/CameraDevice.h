//=================================
// include guard
#ifndef __CAMERADEVICE_H_INCLUDED__
#define __CAMERADEVICE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include "Core/Rot3.h"
#include "Core/Vec3.h"
#include "Core/HomTra3.h"
#include "CameraRay.h"
#include "Image.h"

namespace Visual {

class CameraDevice{
public: 
	CameraDevice(
		const std::string camera_name, 
		const unsigned int sensor_cols, 
		const unsigned int sensor_rows
	);

	virtual void acquire_image(	
		const Frame* world, 
		const Config* visual_config
	) = 0;

	virtual void update_position(const Vec3 new_cam_pos_in_world);

	virtual void update_orientation(const Rot3 new_cam_rot_in_world);

	virtual void update_position_and_orientation(		
		const Vec3 new_cam_pos_in_world,
		const Rot3 new_cam_rot_in_world
	);

	void set_pointing_direction(	
		Vec3 camera_pointing_direction_in_World,
		Vec3 camera_image_upwards_image_dir_in_world
	);

	virtual void set_FoV_in_rad(const double FoV_in_rad);

	double get_FoV_in_rad()const;

	Vec3 get_normalized_pointing_get_direction()const;

	Vec3 direction_to_the_right_of_the_camera()const;

	Ray get_optical_axis_in_world()const;

	std::string get_name()const;

	std::string str()const;

	void save_image(const std::string image_path)const;

	virtual const Image* get_image()const;

	Vec3 get_position_in_world()const;

	Rot3 get_rotation_in_world()const;

	Vec3 get_image_upwards_direction_in_world_frame()const;

	unsigned int get_number_of_sensor_cols()const {
		return image.get_number_of_cols();
	}

	unsigned int get_number_of_sensor_rows()const {
		return image.get_number_of_rows();
	}
protected:
	const std::string CameraName;

	Image image;

	HomTra3 T_Camera2World;
	HomTra3 T_World2Camera;
	Vec3 CameraPositionInWorld;
	Rot3 CameraOrientationInWorld;
	Vec3 CameraPointingDirection;
	CameraRay OpticalAxis;

	double FoV_in_rad;	

	void set_position_and_orientation(	
		const Vec3 cam_pos_in_world,
		const Rot3 cam_rot_in_world
	);

	std::string get_camera_print()const;

	void assert_FoV_is_valid(const double FoV_in_rad)const;

	void update_optical_axis_and_orientation();
};

}//Visual

#endif // __CAMERADEVICE_H_INCLUDED__ 
