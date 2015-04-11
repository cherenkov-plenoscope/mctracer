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
#include "Core/Rotation3D.h"
#include "Core/Vector3D.h"
#include "Core/HomoTrafo3D.h"
#include "CameraRay.h"
#include "CameraImage.h"
#include "TracerException.h"

class CameraDevice{
public: 
	CameraDevice(
		const std::string camera_name, 
		const uint sensor_cols, 
		const uint sensor_rows
	);

	~CameraDevice();

	virtual void acquire_image(	
		const Frame* world, 
		const GlobalSettings* settings
	);

	virtual void update_position(const Vector3D new_cam_pos_in_world);

	virtual void update_orientation(const Rotation3D new_cam_rot_in_world);

	virtual void update_position_and_orientation(		
		const Vector3D new_cam_pos_in_world,
		const Rotation3D new_cam_rot_in_world
	);

	void set_pointing_direction(	
		Vector3D camera_pointing_direction_in_World,
		Vector3D camera_image_upwards_image_dir_in_world
	);

	virtual void set_FoV_in_rad(const double FoV_in_rad);

	double get_FoV_in_rad()const;

	Vector3D get_normalized_pointing_direction()const;

	Vector3D direction_to_the_right_of_the_camera()const;

	Ray get_optical_axis_in_world()const;

	std::string get_name()const;

	std::string get_print()const;

	void save_image(const std::string image_path)const;

	virtual const CameraImage* get_image()const;

	Vector3D get_position_in_world()const;

	Rotation3D get_rotation_in_world()const;

	Vector3D get_image_upwards_direction_in_world_frame()const;

	uint get_number_of_sensor_cols()const {
		return image->get_number_of_cols();
	}

	uint get_number_of_sensor_rows()const {
		return image->get_number_of_rows();
	}
protected:
	const std::string CameraName;

	CameraImage *image;

	HomoTrafo3D T_Camera2World;
	HomoTrafo3D T_World2Camera;
	Vector3D CameraPositionInWorld;
	Rotation3D CameraOrientationInWorld;
	Vector3D CameraPointingDirection;
	CameraRay OpticalAxis;

	double FoV_in_rad;	

	void set_position_and_orientation(	
		const Vector3D cam_pos_in_world,
		const Rotation3D cam_rot_in_world
	);

	std::string get_camera_print()const;

	void assert_FoV_is_valid(const double FoV_in_rad)const;

	void update_optical_axis_and_orientation();
};
#endif // __CAMERADEVICE_H_INCLUDED__ 
