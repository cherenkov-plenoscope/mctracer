//=================================
// include guard
#ifndef __PINHOLECAMERA_H_INCLUDED__
#define __PINHOLECAMERA_H_INCLUDED__

//=================================
// forward declared dependencies
class CartesianFrame;
class GlobalSettings;
//=================================
// included dependencies
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

// parallel computing
#include <omp.h>

#include "Vector3D.h"
#include "CameraDevice.h"

//=================================
class PinHoleCamera:public CameraDevice{
	protected:
	Vector3D PrincipalPointOfSensorPlane;
	Vector3D SensorDirectionU;
	Vector3D SensorDirectionV;
	double FieldOfView_in_Rad;
	double dist_camera_base_2_principal_point;
	// image
	bool CameraImageAlreadyAllocated;
	cv::Mat *CameraImage;
	public:
	//======================
	PinHoleCamera();
	//======================
	~PinHoleCamera();
	//======================
	void set_pin_hole_cam(double fov);
	//======================
	std::string get_pin_hole_cam_string();
	//======================
	void disp();
	//======================
	Ray cam_send_ray(int Uu,int Vv);
	//======================
	void cam_acquire_image_parallel(
	CartesianFrame* world,
	GlobalSettings* settings);
	//======================
	void cam_save_image(std::string str_image_name);
	//======================
	void cam_acquire_image(
	CartesianFrame* world,
	GlobalSettings *settings,
	std::string str_image_name);
	//======================
	cv::Mat get_image()const;
};
#endif // __PINHOLECAMERA_H_INCLUDED__
