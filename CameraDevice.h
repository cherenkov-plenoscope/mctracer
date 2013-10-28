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
#include "Ray.h"
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
	Ray             OpticalAxis;
	public:
	// Sensor
	int SensorResolutionU;
	int SensorResolutionV;
	int SensorResolutionUTimesV;
	//======================
	bool set_cam(	
	std::string new_camera_name, 
	Vector3D 	new_camera_position_in_World,
	Rotation3D 	new_camera_orientation_in_World, 
	int 		newSensorResolutionU,
	int 		newSensorResolutionV);
	//======================
	std::string get_cam_string()const;
	//======================
	Vector3D get_pointing_direction()const;
	//======================
	void disp()const;
};
#endif // __CAMERADEVICE_H_INCLUDED__ 
