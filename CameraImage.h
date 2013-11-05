//=================================
// include guard
#ifndef __CAMERAIMAGE_H_INCLUDED__
#define __CAMERAIMAGE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "CameraDevice.h"

//=================================
class CameraImage{
protected:
	// image
	bool ImageAlreadyAllocated;
	cv::Mat *Image;
	//=================================
public:
	//=================================
	CameraImage();
	//=================================
	~CameraImage();
	//=================================
	void allocate_memory_for_image(
	uint ImageResolutionV,
	uint ImageResolutionU);
	//=================================
	void save_image(std::string str_image_name)const;
	//=================================
	cv::Mat get_image()const;
};
#endif // __CAMERAIMAGE_H_INCLUDED__
