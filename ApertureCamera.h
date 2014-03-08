//=================================
// include guard
#ifndef __APERTURECAMERA_H_INCLUDED__
#define __APERTURECAMERA_H_INCLUDED__

//=================================
// forward declared dependencies
class CartesianFrame;
class GlobalSettings;
//=================================
// included dependencies
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "CameraDevice.h"
#include "CameraImage.h"
//=================================
class ApertureCamera :public CameraDevice, public CameraImage{
protected:
	double FStopNumber;			
	double ApertureRadius_in_m;	
	double FocalLength_in_m; 	
	double ObjectDistance_in_m; 
	double SensorDistance_in_m; 
	double SensorSizeX_in_m; 	
	double SensorSizeY_in_m; 	
	double PixelPitch_in_m; 	

	int NumberOfRaysPerPixelToBeEmitted;
	
	// mt19937 is a standard mersenne_twister_engine
	std::mt19937 pRNG_mt19937;

public:
	//======================
	bool set_object_distance(double n_object_distance);
	//======================
	bool set_aperture_cam(	
	double new_FStopNumber,
	double new_FocalLength,
	double new_SensorSizeX,
	double new_SensorSizeY,
	int new_NumberOfRaysPerPixelToBeEmitted
	);
	//======================
	std::string get_aperture_cam_string();
	//======================
	void disp();
	//======================
	void cam_send_ray(double Xx,double Yy,CameraRay* ray_camera_pixel_x_y);
	//======================
	void cam_acquire_image(
	CartesianFrame* world,
	GlobalSettings* settings
	);
	//======================
	void cam_acquire_stereo_anaglyph_image(
	CartesianFrame* world,
	GlobalSettings* settings,
	double cmaera_offset_in_m);
};
#endif // __APERTURECAMERA_H_INCLUDED__
