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
#include "Rotation3D.h"
#include "Vector3D.h"
#include "HomoTrafo3D.h"
//#include "Ray.h"
#include "CameraRay.h"
//=================================
class CameraDevice{
protected:
	//basic
	std::string     CameraName;
	// position and direction of camera
	HomoTrafo3D     T_Camera2World;
	HomoTrafo3D     T_World2Camera;
	Vector3D        CameraPositionInWorld;
	Rotation3D      CameraOrientationInWorld;
	Vector3D        CameraPointingDirection;
	CameraRay       OpticalAxis;

	// Sensor
	int SensorResolutionU;
	int SensorResolutionV;
	int SensorResolutionUTimesV;
public:
	//======================
	bool set_cam(	
	std::string new_camera_name, 
	Vector3D 	new_camera_position_in_World,
	Rotation3D 	new_camera_orientation_in_World, 
	int 		newSensorResolutionU,
	int 		newSensorResolutionV);
	//======================
	void set_pointing_direction(
	Vector3D new_camera_pointing_direction_in_World,
	Vector3D new_camera_image_upwards_image_orientation_in_world);
	//======================
	std::string get_cam_string()const;
	//======================
	void disp()const;
	//======================
	Vector3D get_pointing_direction()const;
	//======================
	Vector3D get_image_upwards_direction_in_world_frame()const;
	//======================
	void calculate_stereo_parameters(
	const CartesianFrame* world,
	const GlobalSettings* settings,
	const double cmaera_offset_in_m,
	Vector3D &left_camera_position,
	Vector3D &left_camera_pointing_direction,	
	Vector3D &right_camera_position,
	Vector3D &right_camera_pointing_direction)const;
	//======================	
};
#endif // __CAMERADEVICE_H_INCLUDED__ 
