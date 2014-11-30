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
#include "TracerException.h"
#include "ColourProperties.h"

//=================================
class CameraImage{
	const uint cols;
	const uint rows;
public:
	cv::Mat *Image;

	CameraImage(const uint cols, const uint rows);
	
	~CameraImage();
	
	uint get_resolution()const;

	uint get_number_of_cols()const;
	
	uint get_number_of_rows()const;

	void save_image_to_file(std::string str_image_name)const;

	void set_pixel_row_col_to_color(
		const uint row, const uint col, const ColourProperties &color
	);

	double get_width_to_height_ratio()const;

	cv::Mat get_image()const;
};
#endif // __CAMERAIMAGE_H_INCLUDED__
