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
#include "Core/Color.h"

//=================================
class CameraImage{
public:
	
	cv::Mat Image;

	CameraImage();
	CameraImage(const uint cols, const uint rows);
	CameraImage(const CameraImage* image_to_copy_from);
	CameraImage(const std::string filename_of_image);
	uint get_number_of_pixels()const;
	uint get_number_of_cols()const;
	uint get_number_of_rows()const;
	void save(std::string filename)const;
	void load(const std::string filename);
	void set_pixel_row_col_to_color(
		const uint row, const uint col, const Color &color
	);
	Color get_pixel_row_col(const uint row, const uint col)const;
	double get_width_to_height_ratio()const;
	void merge_left_and_right_image_to_anaglyph_3DStereo(
		CameraImage* left_image, CameraImage* right_image
	);
private:

	void convert_to_grayscale();
};
#endif // __CAMERAIMAGE_H_INCLUDED__
